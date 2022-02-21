#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<rosserial_test/SerialPort.h>
using namespace std;

unsigned char vx[4];
unsigned char vy[4];
unsigned char wz[4];
void send_speed(const geometry_msgs::Twist::ConstPtr &speed_msg)
{

        memcpy(vx, &speed_msg->linear.x, 4);
        memcpy(vy, &speed_msg->linear.y, 4);
        memcpy(wz, &speed_msg->angular.z, 4);
        cout<<"speed_vx = "<<speed_msg->linear.x<<endl;
        //根据前面制定的发送贫频率自动休眠 休眠时间 = 1/频率；
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "SerialPort");
    ros::NodeHandle nh;
    geometry_msgs::Twist speed_msg;
    ros::Subscriber speed_sub = nh.subscribe<geometry_msgs::Twist>("tertlesim1/cmd_vel",10, send_speed);

    //串口初始化相关
    SerialPort w;
    bool flag_s = w.open("/dev/ttyUSB0", 115200, 0, 8, 1);
    cout<<"flag_s: "<<flag_s<<endl;
    if(!flag_s)
    {
        ROS_WARN("open serial port failed...");
        return 0;
    }
    //逻辑(一秒10次)
    ros::Rate r(1);

    //节点不死
    while (ros::ok())
    {
        w.send(vx, sizeof(vx));
        w.send(vy, sizeof(vy));
        w.send(wz, sizeof(wz));
    
        r.sleep();

        ros::spinOnce();
    }


    return 0;
}
