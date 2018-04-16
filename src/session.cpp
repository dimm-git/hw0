#include <stdexcept>

#include "session.h"

session::session(iosvc& io_service) : m_socket(io_service), m_buffer(1024)
{
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
    m_socket.async_read_some(asio_buffer(m_buffer.data(), m_buffer.size()), boost::bind(&session::handle_read, this, ph_error, ph_trans));
    // boost::asio::async_read(m_socket, asio_buffer(m_buffer.data(), m_buffer.size()), boost::bind(&session::handle_read, this, ph_error, ph_trans));
}

void session::handle_read(const error_code& error, std::size_t bytes_transferred)
{
    if (bytes_transferred > 0)
        /* m_parser->process(reinterpret_cast<const char*>(m_buffer.data()), bytes_transferred) */ ;

    if (error)
        delete this;
    else
        m_socket.async_read_some(asio_buffer(m_buffer.data(), m_buffer.size()), boost::bind(&session::handle_read, this, ph_error, ph_trans));
}
