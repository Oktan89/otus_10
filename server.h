#pragma once
#include <boost/asio.hpp>
#include "bulk_lib/async.h"
#include <iostream>
#include <memory>



using b_tcp = boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(b_tcp::socket socket, const async::handle_t& bulk) 
        : m_socket(std::move(socket)), m_bulk(bulk)
    {

    }

    void start()
    {
        do_read();
    }

private:

    void do_read()
    {
        auto self(shared_from_this());
        m_socket.async_read_some(boost::asio::buffer(m_data, m_length), 
        [this, self](const boost::system::error_code& ec, std::size_t length)
        {
            if(!ec)
            {
                async::receive(m_bulk, m_data, length);
            }
            do_read();
        });
    }
b_tcp::socket m_socket;
enum { m_length = 1024 };
char m_data[m_length];
const async::handle_t& m_bulk;
};

class Server
{
public:
    Server(boost::asio::io_context& context, short port, std::size_t bulk)
        : m_acceptor(context, b_tcp::endpoint(b_tcp::v4(), port)), serv_bulk(async::connect(bulk)) 
    {
        do_accept();
    }
    ~Server()
    {
        async::disconnect(serv_bulk);
    }
private:

    void do_accept()
    {
        m_acceptor.async_accept([this](const boost::system::error_code& ec, b_tcp::socket socket){
            if(!ec)
            {
                std::make_shared<Session>(std::move(socket), serv_bulk)->start();
            }
            do_accept();
        });
    }
b_tcp::acceptor m_acceptor;
async::handle_t serv_bulk;  
};