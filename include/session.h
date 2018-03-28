#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <mutex>
#include <vector>

#include "async_config.h"
#include "chunk_parser.h"
#include "input_handler.h"

using iosvc = boost::asio::io_service;
using asio_socket = boost::asio::ip::tcp::socket;
using error_code = boost::system::error_code;
#define asio_buffer boost::asio::buffer
#define ph_error boost::asio::placeholders::error
#define ph_trans boost::asio::placeholders::bytes_transferred

class async_provider
{
    private:
        std::size_t m_size;
        std::list<std::unique_ptr<async_config> > m_wait;
        std::mutex m_lock;

    public:
        async_provider(std::size_t bulk_size);
        async_config* find();
        void reserve();
        void release(async_config* cfg);
        void squeeze(async_config* cfg);
};

class bulk_adapter;

class session : private input_handler
{
  public:
    session(iosvc& io_service, async_provider* provider);
    virtual ~session() noexcept;

    asio_socket& get_socket();
    void start();

  private:
    void handle_read(const error_code& error, std::size_t bytes_transferred);

    bool next(const std::string& s) override;
    void flush() override;

    void done();

    asio_socket m_socket;
    std::vector<std::int8_t> m_buffer;
    std::unique_ptr<bulk_adapter> m_parser;
    async_provider* m_provider;
    std::unique_ptr<async_config> m_config;
    std::size_t m_level = 0;
};
