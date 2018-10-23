#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <netdb.h>
#include <vector>

using namespace std;

std::vector<char> buffer(256);

int main() {
    cout << "CLIENT\n";
    string address;
    cout << "Input server address in format: <ip>:<port>\n"
            "For example: 132.32.3.232:54346\n"
            "Or input 't' for test value 0.0.0.0:22222\n"
            "Address: ";
    getline(cin, address);
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

    hostent *server = gethostbyname(&IP[0]);
    if (server == nullptr) {
        cerr << "Can't find such server\n";
        return 0;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    memcpy(server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Connecting error\n";
        return 0;
    }

    cout << "Connecting to server: " << address << "\n";
    cout << "Input ':stop' to stop client\n";

    string message;
    while (true) {
        cout << "Input message: ";
        getline(cin, message, '\n');

        if (write(sockfd, &message[0], strlen(&message[0])) < 0) {
            cerr << "Writing error\n";
            return 0;
        }

        if (message == ":stop") {
            break;
        }

        if (message == ":s") {
            break;
        }

        buffer.assign(256, 0);

        if (recv(sockfd, &buffer[0], 256, 0) < 0) {
            cerr << "Reading error\n";
            return 0;
        }
        cout << "Message from server: " << &buffer[0] << "\n";
    }
    cout << "Disconnecting from server: " << address << "\n";

    close(sockfd);
    return 0;
}