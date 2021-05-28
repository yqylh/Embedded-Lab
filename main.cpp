#include "./lib/include.cpp"
#include "./lib/badapple.cpp"
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>

void handle_request(http_request_s * request) {
    http_request_connection(request,HTTP_AUTOMATIC);
    http_response_s* response = http_response_init();
    http_response_status(response, 200);
    if (request_target_is(request,"/badapple")){
        // http_string_s body = http_request_body(request);
        // std::cout<<get_body_string(body)<<std::endl;
        static int PID = 0;
        if (PID != 0) kill(PID , SIGINT);
        PID = fork();
        if (PID == 0) {
            badapple();
            exit(0);
        }
        http_response_header(response,"Content-Type", "text/plain");
        http_response_body(response,"OK", 2);
    } else {
        http_response_status(response, 404);
    }
    http_respond(request,response);
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