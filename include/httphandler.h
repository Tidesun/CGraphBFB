#ifndef HANDLER_H
#define HANDLER_H
#include <iostream>
#include "stdafx.h"
#include "bfb_algorithm.hpp"
//#include "../dbms/include/Dbms.h"

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;


class httphandler
{
public:
    httphandler();
    httphandler(utility::string_t url);
    virtual ~httphandler();

    pplx::task<void>open(){return m_listener.open();}
    pplx::task<void>close(){return m_listener.close();}

protected:

private:
    void handle_get(http_request message);
    void handle_put(http_request message);
    void handle_post(http_request message);
    void handle_delete(http_request message);
    void handle_error(pplx::task<void>& t);
    http_listener m_listener;
};

#endif // HANDLER_H