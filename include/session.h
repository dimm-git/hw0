#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <mutex>
#include <vector>

#include "database.h"
#include "lock_manager.h"

using iosvc = boost::asio::io_service;
using asio_socket = boost::asio::ip::tcp::socket;
using error_code = boost::system::error_code;
#define asio_buffer boost::asio::buffer
#define ph_error boost::asio::placeholders::error
#define ph_trans boost::asio::placeholders::bytes_transferred

class session
{
  public:
    session(iosvc& io_service, database& db, lock_manager& lm);
    virtual ~session() noexcept;

    asio_socket& get_socket();
    void start();

  private:
    void handle_read(const error_code& error, std::size_t bytes_transferred);
    void process_command(const std::string& cmd);

    asio_socket m_socket;
    boost::asio::streambuf m_buffer;
    database& m_db;
    lock_manager& m_lm;
};
