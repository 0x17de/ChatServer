#ifndef CLIENT_H
#define CLIENT_H


class Client {
private:
    int fd_;
public:
    Client(int fd);
    virtual ~Client();

    int getFd();
};

#endif // CLIENT_H
