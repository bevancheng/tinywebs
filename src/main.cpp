#include <unistd.h>
#include "../include/server/webserver.h"

int main()
{

    WebServer server(9006, 3, 60000, false, 3306, "ubuntu", "ubuntu", "yourdb", 12, 6, true,1, 1024);
    server.Start();
}