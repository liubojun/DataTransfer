#ifndef record_h__
#define record_h__

#include <QMap>
#include <QString>

typedef struct RecordFileInfo
{
    QString m_strFileName; // 文件名称
    QString m_strFileDt;   // 文件时间
    int	    m_iFileSize;   // 文件大小

    RecordFileInfo(const QString &fn, const QString &dt, int isize) : m_strFileName(fn), m_strFileDt(dt), m_iFileSize(isize)
    {

    }

    RecordFileInfo(const QString &dt, int isize) : m_strFileDt(dt), m_iFileSize(isize)
    {

    }

    RecordFileInfo()
    {

    }

} recordFileInfo_t;

// 在内存中维护的文件名+文件属性列表
typedef QMap<QString, recordFileInfo_t> dir2filetable_t;

// 在内存中维护的目录名+当前目录下的文件信息
typedef QMap<QString, dir2filetable_t> memfiletable_t;

class CDirRecord
{
public:
    /**
     * @brief  带参构造函数
     * @param  const QString &dirid 需要查询的ID
     */
    CDirRecord(const QString &dirid);




    /**
    * @brief  更新目录的上一次处理的文件的列表以及文件
    *         考虑到远程的ftp服务器的版本差异，获取远程的文件的时间不太靠谱
    *		   只能通过大小来进行比对，如果本次获取的大小与上一次的发生了差异，则可以认为文件被更新了
    * @param  const QString &dir 目录名
    * @param  const QString &filename 文件名
    * @param  const QString &dt 文件最后修改时间
    * @param  const QString &ifilesize 文件大小
    * @return void
    */
    void updateLatestFileSize(const QString &dir, const QString &filename, const QString &dt, int ifilesize);

    /**
     * @brief  检查当前文件是否在上次处理的文件列表中
     *		   根据文件名，文件的最后修改时间，以及文件的大小属性来判断
     * @param  const QString &dir 文件目录
     * @param  const QString &fn 文件名
     * @param  const QString &dt 文件时间
     * @param  int size 文件大小
     * @return bool：如果在上次处理的列表中，返回false,否则返回true
     */
    bool checkIsNewFile(const QString &dir, const QString &fn, const QString &dt, int size);

	/**
	 * @brief  检查当前文件是否在上次处理的文件列表中
	 *		   根据文件名，文件的最后修改时间，以及文件的大小属性来判断
	 * @param  const QString &dir 文件目录
	 * @param  const QString &fn 文件名
	 * @return bool：如果在上次处理的列表中，返回false,否则返回true
	 */
	void updateSendFailure(const QString &dir, const QString &fn);

    /**
     * @brief  查询目录的上一次处理的文件的列表以及文件，加载到内存中
     *         考虑到远程的ftp服务器的版本差异，获取远程的文件的时间不太靠谱
     *		   只能通过大小来进行比对，如果本次获取的大小与上一次的发生了差异，则可以认为文件被更新了
     * @return void：无
     */
    void loadLatestFileSize();

    /**
     * @brief  刷新本次记录内容到配置文件中
     * @return void：无
     */
    void reflush();

private:
    /**
     * @brief  更新目录的上一次处理的文件的列表以及文件
     *         考虑到远程的ftp服务器的版本差异，获取远程的文件的时间不太靠谱
     *		   只能通过大小来进行比对，如果本次获取的大小与上一次的发生了差异，则可以认为文件被更新了
     * @param  memfiletable_t：文件列表以及对应的文件大小
     * @return bool：更新成功，返回true，失败返回false
     */
    bool updateLatestFileSize(memfiletable_t &oFileSizeInfo);

private:
    QString m_strUrl;

    memfiletable_t m_oMemLastFileInfo;
    //QMap<QString, RecordFileInfo> m_oMemLastFileInfo;

    memfiletable_t m_oMemThisFileInfo;
    //QMap<QString, RecordFileInfo> m_oMemThisFileInfo;
};
#endif // record_h__
