#include "./lib/include.cpp"
#include "./lib/badapple.cpp"
#include "./lib/number.cpp"
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <pthread.h>
#include "./lib/led.cpp"

void handle_request(http_request_s *request)
{
    http_request_connection(request, HTTP_AUTOMATIC);
    http_response_s *response = http_response_init();
    http_response_status(response, 200);
    http_string_s body = http_request_body(request);
    if (request_target_is(request, "/led")) {
        jsonxx::json j = jsonxx::json::parse(get_body_string(body));
        int status = std::stoi(std::string(j["status"])), num = std::stoi(std::string(j["num"])); 
        led(status, num);
        set_response_ok(response);
    } else if (request_target_is(request, "/nfc/init"))  {
        if (nfc != nullptr) delete nfc;
        nfc = new NFC();
        if (nfc->initNFC())  set_response_ok(response);
        else set_response_fail(response);
    }
    else if (request_target_is(request, "/nfc/wakeup")) {
        if (nfc != nullptr) {
            nfc->WakeUpNFC();
            set_response_ok(response);
        } else set_response_fail(response);
    }
    else if (request_target_is(request, "/nfc/label")) {
        if (nfc != nullptr) {
            std::string temp = nfc->LabelNFC();
            http_response_header(response, "Content-Type", "text/plain");
            http_response_body(response, temp.c_str(), temp.length());
        } else set_response_fail(response);
    } else if (request_target_is(request, "/nfc/close")) {
        if (nfc != nullptr) delete nfc;
        nfc = nullptr;
        set_response_ok(response);
    } else if (request_target_is(request, "/badapple")) {
        pthread_t th;
        int ret;
        int *thread_ret = NULL;
        ret = pthread_create(&th, NULL, badapple, NULL);
        // std::thread badappleThread(badapple);
        // badappleThread.detach();
        // static int PID = 0;
        // if (PID != 0) kill(PID , SIGINT);
        // PID = fork();
        // std::cout << PID << std::endl;
        // if (PID == 0) {
        //     badapple();
        //     exit(0);
        // }
        http_response_header(response, "Content-Type", "text/plain");
        http_response_body(response, "OK", 2);
    }
    else if (request_target_is(request, "/number"))  {
        jsonxx::json j = jsonxx::json::parse(get_body_string(body));
        int num = std::stoi(std::string(j["num"])); 

        pthread_t th;
        int ret;
        int *thread_ret = NULL;
        // int arg = 12345678;
        int arg = num;
        ret = pthread_create(&th, NULL, number, (void *)&arg);
        http_response_header(response, "Content-Type", "text/plain");
        http_response_body(response, "OK", 2);
    }
    else {
        http_response_status(response, 404);
    }
    http_respond(request, response);
}

int main() {
    init();
#ifdef EBUG
    test();
    return 0;
#endif
    server = http_server_init(8080, handle_request);
    http_server_listen(server);
    return 0;
}