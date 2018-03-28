#include <stdexcept>

#include "async_config.h"
#include "chunk_parser.h"

#include "session.h"

async_provider::async_provider(std::size_t bulk_size) : m_size(bulk_size)
{
}

async_config* async_provider::find()
{
    std::unique_lock<std::mutex> guard(m_lock);
    if (m_wait.size() == 0)
        return new async_config(m_size);
    async_config* front = m_wait.front().release();
    m_wait.erase(m_wait.begin());
    return front;
}

void async_provider::reserve()
{
    std::unique_lock<std::mutex> guard(m_lock);
    m_wait.push_back(std::unique_ptr<async_config>(new async_config(m_size)));
}

void async_provider::release(async_config* cfg)
{
    std::unique_lock<std::mutex> guard(m_lock);
    m_wait.push_back(std::unique_ptr<async_config>(cfg));
}

void async_provider::squeeze(async_config* cfg)
{
    delete cfg;
}

session::session(iosvc& io_service, async_provider* provider) : m_socket(io_service), m_buffer(1024), m_provider(provider)
{
    m_parser.reset(new bulk_adapter(this));
    m_provider->reserve();
}

session::~session() noexcept
{
}

asio_socket& session::get_socket()
{
    return m_socket;
}

void session::start()
{
    // m_socket.async_read_some(asio_buffer(m_buffer.data(), m_buffer.size()), boost::bind(&session::handle_read, this, ph_error, ph_trans));
    boost::asio::async_read(m_socket, asio_buffer(m_buffer.data(), m_buffer.size()), boost::bind(&session::handle_read, this, ph_error, ph_trans));
}

void session::handle_read(const error_code& error, std::size_t bytes_transferred)
{
    if (bytes_transferred > 0)
        m_parser->process(reinterpret_cast<const char*>(m_buffer.data()), bytes_transferred);

    if (error)
    {
        m_parser->done();
        done();
        delete this;
    }
}

bool session::next(const std::string& line)
{
    std::unique_ptr<async_config> guard;
    async_config* config = nullptr;
    if (m_config.get() == nullptr)
    {
        config = m_provider->find();
        guard.reset(config);
    }
    else
    {
        config = m_config.get();
    }
    total_stats* ts = config->app_stats.get();
    input_handler* h = config->inp_handler.get();
    if (line == "{")
    {
        if (m_config.get() == nullptr)
            m_config.reset(guard.release());
        m_level++;
    }
    else if ((line == "}") && (m_level > 0))
    {
        m_level--;
        if (m_level == 0)
            guard.reset(m_config.release());
    }
    h->next(line);
    ts->line_read();
    if (m_config.get() == nullptr)
        m_provider->release(guard.release());
    return true;
}

void session::flush()
{
    throw std::runtime_error("unexpected session::flush() call");
}

void session::done()
{
    std::unique_ptr<async_config> config(m_config.get() == nullptr ? m_provider->find() : m_config.release());

    if (config->inp_handler.is_null() == false)
        config->inp_handler->flush();
    if (config->con_logger.is_null() == false)
        config->con_logger->done();
    if (config->file_logger.is_null() == false)
        config->file_logger->done();
}
