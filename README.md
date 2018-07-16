# DataTransfer
数据传输系统，支持共享目录以及ftp数据的采集和分发
依赖第三库：libcurl以及boost

在函数 static CURLcode ftp_parse_url_path(struct connectdata *conn) 中 添加如下代码

		if (ftpc->dirdepth == 0)
		{
			ftpc->dirs[0] = strdup("/");
			ftpc->dirdepth++;
		}
