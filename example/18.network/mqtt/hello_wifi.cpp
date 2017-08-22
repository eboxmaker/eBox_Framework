#include "ebox.h"

#include "WifiIPStack.h"
#include "Countdown.h"
#include "MQTTClient.h"

WIFI_TCP tcp(&wifi);
WifiIPStack ipstack(&tcp);

MQTT::Client<WifiIPStack, Countdown> client = MQTT::Client<WifiIPStack, Countdown>(ipstack);

int arrivedcount = 0;

void messageArrived(MQTT::MessageData& md);
void connect(void);

const char* topic = "eBox-sample";
//u8 rip[4] = {198,41,30,241};
u8 rip[4] = {192,168,1,197};

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("MQTT Hello example\n");
    wifi.begin();
    wifi.join_ap();
    connect();
}

int main()
{
    setup();
    while(1)
    {
        if (!client.isConnected())
            connect();

        MQTT::Message message;

        arrivedcount = 0;

        // Send and receive QoS 0 message
        char* buf = (char*)"Hello, eBox\n";
        uart1.printf("Hello World! QoS 0 message\n");
        message.qos = MQTT::QOS0;
        message.retained = false;
        message.dup = false;
        message.payload = (void*)buf;
        message.payloadlen = strlen(buf) + 1;
        int rc = client.publish(topic, message);
        if (rc != 0 )
            uart1.printf("Error %d from sending QoS 0 message\n", rc);
        else while (arrivedcount == 0)
                client.yield(1000);

        //delay_ms(500);


        // Send and receive QoS 1 message
        uart1.printf("Hello World!  QoS 1 message\n");
        message.qos = MQTT::QOS1;
        message.payloadlen = strlen(buf) + 1;
        rc = client.publish(topic, message);
        if (rc != 0 )
            uart1.printf("Error %d from sending QoS 1 message\n", rc);
        else while (arrivedcount == 1)
                client.yield(1000);


        //delay_ms(500);

        // Send and receive QoS 2 message
        uart1.printf("Hello World!  QoS 2 message\n");
        message.qos = MQTT::QOS2;
        message.payloadlen = strlen(buf) + 1;
        rc = client.publish(topic, message);
        if (rc != 0 )
            uart1.printf("Error %d from sending QoS 2 message\n", rc);
        else while (arrivedcount == 2)
                client.yield(1000);

        //delay_ms(500);
    }

    return 0;
}

void connect()
{
    char hostname[] = "iot.eclipse.org";
    //char hostname[] = "192.168.1.197";
    int port = 1883;
    uart1.printf("Connecting to %s:%d\n", hostname, port);
    int rc = ipstack.connect(hostname, port);
    if (rc != 1)
    {
        uart1.printf("rc from TCP connect is %d\n", rc);
    }

    uart1.printf("MQTT connecting\n");
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = (char*)"eBox-sample";
    rc = client.connect(data);
    if (rc != 0)
    {
        uart1.printf("rc from MQTT connect is %d\n", rc);
    }
    uart1.printf("MQTT connected\n");

    rc = client.subscribe(topic, MQTT::QOS2, messageArrived);
    if (rc != 0)
    {
        uart1.printf("rc from MQTT subscribe is %d\n", rc);
    }
    uart1.printf("MQTT subscribed\n");
}


void messageArrived(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;
    uart1.printf("Message %d arrived: qos %d, retained %d, dup %d, packetid %d\n",
                 ++arrivedcount, message.qos, message.retained, message.dup, message.id);
    uart1.printf("Payload %s\n", (char*)message.payload);
}

