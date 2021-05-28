#include "web.hpp"
#include "nfc/nfc.hpp"
http_server_s * server;

NFC* nfc;


void handle_request(http_request_s * request){
    http_request_connection(request,HTTP_AUTOMATIC);
    http_response_s* response = http_response_init();

    http_response_status(response,200);

    if(request_target_is(request,"/nfc/init")){
        if(nfc != nullptr)
            delete nfc;
        nfc = new NFC();
        if (nfc->initNFC())
            set_response_ok(response);
        else
            set_response_fail(response);
    }
    else if(request_target_is(request,"/nfc/wakeup")){
        if(nfc!=nullptr){
            nfc->WakeUpNFC();
            set_response_ok(response);
        }
        else
            set_response_fail(response);
    }
    else if(request_target_is(request,"/nfc/label")){
        if(nfc!=nullptr){
            std::string temp = nfc->LabelNFC();
            http_response_header(response,"Content-Type", "text/plain");
            http_response_body(response,temp.c_str(),temp.length()); 
            // set_response_ok(response);
        }
        else 
            set_response_fail(response);
    }
    else if(request_target_is(request,"/nfc/close")){
        if(nfc!=nullptr)
            delete nfc;
        nfc = nullptr;
        set_response_ok(response);
    }

    http_respond(request, response);
}

int main(){
    server = http_server_init(8080,handle_request);
    http_server_listen(server);
    return 0;
}

