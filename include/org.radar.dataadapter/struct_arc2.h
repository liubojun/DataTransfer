#ifndef ARC2STRUCT_H_
#define ARC2STRUCT_H_

#pragma pack(1)

typedef struct
{
    unsigned short int CTM[6]; //CTM信息 （Channel Terminal Manager）
    //记录信息（Message Header）结构
    unsigned short int MessageSize;  //12~13 表示记录长度，双字节单位表示
    char ChannelID;    //14    信号传输通道号(=0,1,2)
    char MessageType;  //15    记录数据类型,具体说明见6.1
    short int IDSequence;  //16~17  记录顺序号，0~0x7FFF之间循环表示
    unsigned short int MessageGenerationDate; //18~19 记录产生日期,以1/1/1970以来
                                              //的 Julian日期表示
    unsigned long  int MessageGenerationTime; //20~23 记录产生的GMT时间(毫秒)
    short int MessageSegmentsNumber; //24~25 记录传输分段段数
    short int MessageSegmentNumber; //26~27 记录传输段号
    //雷达数据头（Digital Radar Data Header）结构
	unsigned long  int RadialCollectionTime; //28~31 径向资料采集的GMT时间(毫秒)
    unsigned short int RadialCollectionDate; //32~33 径向资料采集日期,以1/1/1970
                                             //以来的Julian日期表示
    unsigned short int UnambiguousRange;  //34~35 不模糊距离,单位：0.1Km
    unsigned short int AzimuthAngle; //36~37 方位角,实际值=(value>>3)*0.043945(度)
    short int AzimuthNumber;     //38~39 本层仰角扫描的径向数
    short int RadialStatus;  //40~41 径向位置参数(=0,1,2,3,4)
    short int ElevationAngle;//42~43 仰角, 实际值=(value>>3)*0.043945(度)
    short int ElevationNumber;   //44~45 体积扫描仰角数
    short int FirstGateRangeOfRef;//46~47第一个强度库的距离(米)
    short int FirstGateRangeOfDoppler;//48~49 第一个速度\谱宽库的距离(米)
    short int ReflectivityGateSize; //50~51 强度库长(米)
    short int DopplerGateSize; //52~53速度\谱宽库长(米)
    short int ReflectivityGates;//54~55	强度库数
    short int DopplerGates;//56~57	速度\谱宽库数
    short int SectorNumber;//58~59	cut内的扇区数
    float CalibrationConstant;//60~63 系统增益标校常数（dB）
    unsigned short int RefPointer;//64~65从雷达数据头到强度数据开始的字节数
    unsigned short int VelPointer;//66~67从雷达数据头到速度数据开始的字节数
    unsigned short int SWPointer;//68~69从雷达数据头到谱宽数据开始的字节数
    short int VelResolution;//70~71 速度分辨率:2=0.5m/s,4=1.0m/s
    short int VCP;//72~73体扫VCP方式（11、21、31、32）
    short int Unused[4];//74~81 未用。
    unsigned short int RefPlaybackPointer;//82~83强度数据指针（用于RDA回放）
    unsigned short int VelPlaybackPointer;//84~85 速度数据指针（用于RDA回放）
    unsigned short int SWPlaybackPointer;//86~87	谱宽数据指针（用于RDA回放）
    short int NyquistVelocity;//88~89 Nyquist速率(0.01m/s)
    short int AAF;//90~91 Atmospheric Attenuation Factor(0.001 dB/Km)
    short int TOVER;//92~93没有距离折叠两个回波功率之间最小差值(0.1瓦）
    short int Spares[17];//94~127 未用。
    //基本数据（Radar Base Data）
    //reflectivity data 460bytes
    unsigned char dBZ[2300];
    //Velocity data 920bytes
    //unsigned char Vel[920];
    //Spectrum Width data 920bytes
    //unsigned char SW[920];
	//帧继编码信息
    unsigned long  int Trailer;
}DATAPACKET;

#endif /*ARC2STRUCT_H_*/
