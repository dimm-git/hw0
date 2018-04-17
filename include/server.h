#pragma once

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "database.h"
#include "lock_manager.h"
#include "session.h"

using acceptor = boost::asio::ip::tcp::acceptor;
using endpoint = boost::asio::ip::tcp::endpoint;
#define ip_type boost::asio::ip::tcp::v4

class server
{
  public:
    server(iosvc& io_service, short port);

  private:
    void start_accept();
    void handle_accept(session* new_session, const error_code& error);

    iosvc& m_svc;
    acceptor m_acceptor;
    database m_db;
    lock_manager m_lm;
};
