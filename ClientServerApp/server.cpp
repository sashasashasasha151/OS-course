#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <vector>

using namespace std;

std::vector<char> buffer(256);

int main() {
    cout << "SERVER\n";
    string address;
    cout << "Input address in format: <ip>:<port>\n"
            "For example: 132.32.3.232:54346\n"
            "Or input 't' for test value 0.0.0.0:22222\n"
            "Address: ";
    cin >> address;
    cout << "\n";

    if (address == "t") {
        address = "0.0.0.0:22222";
    }

    unsigned int i = 0;
    while (address[i++] != ':') {}

    string IP = address.substr(0, i - 1);

    int port = stoi(address.substr(i, address.length() - i));

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr << "Can't open socket\n";
        return 0;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(&IP[0]);
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Binding error\n";
        return 0;
    }

    if (listen(sockfd, 1) < 0) {
        cerr << "Listening error\n";
        return 0;
    }

    cout << "Server started\n";
    cout << "Send ':s' from client to stop server\n\n";

    while (true) {

        sockaddr_in cli_addr{};
        socklen_t clilen = sizeof(cli_addr);

        int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            cerr << "Accepting error\n";
            return 0;
        }

        string client_adress = inet_ntoa(cli_addr.sin_addr);
        client_adress += ":" + to_string(ntohs(cli_addr.sin_port));

        cout << "Client " << client_adress << " connecting\n";


        bool out = false;

        while (true) {


            buffer.assign(256, 0);

            if (recv(newsockfd, &buffer[0], 256, 0) <= 0) {
                cerr << "Reading error\n";
                break;
            }

            if (strcmp(&buffer[0], ":s") == 0) {
                out = true;
                break;
            }

            if (strcmp(&buffer[0], ":stop") == 0) {
                break;
            }

            cout << "Get message from client: " << &buffer[0] << "\n";

            cout << "Send message to client: " << client_adress << "\n";
            if (send(newsockfd, &buffer[0], strlen(&buffer[0]), 0) < 0) {
                cerr << "Wring error\n";
                break;
            }
        }
        close(newsockfd);
        cout << "Client " << client_adress << " disconnecting\n\n";

        if (out) {
            break;
        }
    }

    close(sockfd);
    cout << "Server finished\n";
    return 0;
}
