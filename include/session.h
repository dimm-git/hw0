#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <mutex>
#include <vector>

using iosvc = boost::asio::io_service;
using asio_socket = boost::asio::ip::tcp::socket;
using error_code = boost::system::error_code;
#define asio_buffer boost::asio::buffer
#define ph_error boost::asio::placeholders::error
#define ph_trans boost::asio::placeholders::bytes_transferred

class session
{
  public:
    session(iosvc& io_service);
    virtual ~session() noexcept;

    asio_socket& get_socket();
    void start();

  private:
    void handle_read(const error_code& error, std::size_t bytes_transferred);

    asio_socket m_socket;
    std::vector<std::int8_t> m_buffer;
};
