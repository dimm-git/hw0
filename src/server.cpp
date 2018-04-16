#include "server.h"

using acceptor = boost::asio::ip::tcp::acceptor;
using endpoint = boost::asio::ip::tcp::endpoint;
#define ip_type boost::asio::ip::tcp::v4

server::server(iosvc& io_service, short port) : m_svc(io_service), m_acceptor(io_service, endpoint(ip_type(), port))
{
    start_accept();
}

void server::start_accept()
{
    session* new_session = new session(m_svc);
    m_acceptor.async_accept(new_session->get_socket(), boost::bind(&server::handle_accept, this, new_session, ph_error));
}

void server::handle_accept(session* new_session, const error_code& error)
{
    if (!error)
        new_session->start();
    else
        delete new_session;
    start_accept();
}
