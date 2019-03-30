#include "httphandler.h"

httphandler::httphandler()
{
    //ctor
}
httphandler::httphandler(utility::string_t url):m_listener(url)
{
    m_listener.support(methods::GET, std::bind(&httphandler::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&httphandler::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&httphandler::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&httphandler::handle_delete, this, std::placeholders::_1));

}
httphandler::~httphandler()
{
    //dtor
}

void httphandler::handle_error(pplx::task<void>& t)
{
    try
    {
        t.get();
    }
    catch(...)
    {
        // Ignore the error, Log it if a logger is available
    }
}


//
// Get Request
//
void httphandler::handle_get(http_request message)
{
    ucout <<  message.to_string() << endl;

    auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));

    message.relative_uri().path();
    //Dbms* d  = new Dbms();
    //d->connect();
    message.reply(status_codes::OK," ");
    return;

};

//
// A POST request
//
void httphandler::handle_post(http_request message)
{
    ucout <<  message.to_string() << endl;
    auto fileStream = std::make_shared<concurrency::streams::ostream>();
    concurrency::streams::ostream outFile = concurrency::streams::fstream::open_ostream(U("../static/graph.dat")).get();
    *fileStream = outFile;
    message.body().read_to_end(fileStream->streambuf()).get();
    fileStream->close().get();
    message.reply(status_codes::OK,message.to_string());
    return ;
};

//
// A DELETE request
//
void httphandler::handle_delete(http_request message)
{
    ucout <<  message.to_string() << endl;

    string rep = U("WRITE YOUR OWN DELETE OPERATION");
    message.reply(status_codes::OK,rep);
    return;
};


//
// A PUT request
//
void httphandler::handle_put(http_request message)
{
    ucout <<  message.to_string() << endl;
    string rep = U("WRITE YOUR OWN PUT OPERATION");
    message.reply(status_codes::OK,rep);
    return;
};

