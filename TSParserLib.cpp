#include "include.h"
#include "TSParserLib.h"
#include "TSPacket.h"

using namespace std;

CTransportStreamParser::CTransportStreamParser(const char *file_name)
{
	if (file_name)
	{
		m_file_name = file_name;
	}

	m_file_size = 0;
#if DUMP_TS_INFO_ENABLED_LOG
	g_logoutFile.open("Log.txt");
#endif
}
CTransportStreamParser::~CTransportStreamParser()
{
#if DUMP_TS_INFO_ENABLED_LOG
	g_logoutFile.close();
#endif
}

//*************** Public API ***************
int CTransportStreamParser::Parse()
{
	int err = kTSParserError_NoError;
	err = open_input_file();
	if (err < 0)
	{
		return err;
	}

	err = get_input_file_data();
	if (err < 0)
	{
		return err;
	}
	Dump_ts_info();

	for (int idx = 0; idx < m_packet_count; idx++)
	{
		BYTE *pkt_buf = m_file_buf + 188 * idx;
		reset_ctx(pkt_buf);
		CTransportStreamPacket pkt;
		err = pkt.Parse_one_ts_packet();		
		if (err < 0)
		{
			break;
		}
		pkt.Dump_packet_info(idx);
	}

	return err;
}
int CTransportStreamParser::Dump_ts_bytes(int numberOfBytes, bool last)
{   
	
	int err = kTSParserError_NoError;
	err = open_input_file();
	if (err < 0)
	{
		return err;
	}
	if(last)
	{  

		//reading last bytes
		int bytesRead = 0;
		int i;
		char buf[m_file_size] = {0};
		/*Find out the size of the file*/
		m_in_file_stream.seekg(0, ios_base::end);
		m_file_size = m_in_file_stream.tellg();
		m_in_file_stream.seekg(0, ios_base::beg);
		if(m_file_size<numberOfBytes){
			cout<<"File too small! ... Aborting\n";
			close_input_file();	
			return kTSParserError_ReadInputFileFailed;
		}
		else
		{
		   /*Read the last bytes of the file*/
		   m_in_file_stream.seekg(-numberOfBytes,ios_base::end);
		   bytesRead = m_in_file_stream.read(buf,sizeof(char),numberOfBytes);
		   printf("Read %d bytes from %s, expected %d\n",bytesRead,m_file_name,numberOfBytes);
		   if(bytesRead>0){
			   for(i;i<bytesRead;i++){
				   printf("%c".buf[i]);
			   }
		   }
		}	
			
	}
	else
	{
		//reading first bytes
	}
	close_input_file();
	return err;
}
int CTransportStreamParser::Dump_ts_packet_info(int index)
{
    int err = kTSParserError_NoError;
	err = open_input_file();
	if (err < 0)
	{
		return err;
	}
	err = get_input_file_data();
	if (err < 0)
	{
		return err;
	}
		BYTE *pkt_buf = m_file_buf + 188 * index;
		reset_ctx(pkt_buf);
		CTransportStreamPacket pkt;
		err = pkt.Parse_one_ts_packet();		
		if (err < 0)
		{
			break;
		}
		pkt.Dump_packet_info(index);
}
void CTransportStreamParser::Dump_ts_info()
{
#if DUMP_TS_INFO_ENABLED_CONSOLE
	cout << "***********************************" << endl;
	cout << "File Name: " << m_file_name << endl;
	cout << "File Size: " << to_string(m_file_size) << endl;
	cout << "***********************************" << endl;
#endif

# if DUMP_TS_INFO_ENABLED_LOG
	g_logoutFile << "***********************************" << endl;
	g_logoutFile << "File Name: " << m_file_name << endl;
	g_logoutFile << "File Size: " << to_string(m_file_size) << endl;
	g_logoutFile << "***********************************" << endl;
#endif
}

//*************** Private Methods ***************
int CTransportStreamParser::open_input_file()
{
	if (!m_file_name)
	{
		return kTSParserError_NullFileName;
	}

	m_in_file_stream.open(m_file_name, ios_base::binary);
	if (!m_in_file_stream.is_open())
	{
		return kTSParserError_OpenInputFileFailed;
	}

	return kTSParserError_NoError;
}

int CTransportStreamParser::get_input_file_data()
{
	m_in_file_stream.seekg(0, ios_base::end);
	m_file_size = m_in_file_stream.tellg();
	m_in_file_stream.seekg(0, ios_base::beg);
	m_packet_count = m_file_size / 188;

	m_file_buf = new BYTE[m_file_size];
	m_in_file_stream.read(reinterpret_cast<char *>(m_file_buf), m_file_size);
	if ((m_in_file_stream.rdstate() & ifstream::failbit) || (m_in_file_stream.rdstate() & ifstream::badbit))
	{
		close_input_file();
		return kTSParserError_ReadInputFileFailed;
	}

	f_ctx.file_buf = m_file_buf;
	f_ctx.byte_position = 0;

	return kTSParserError_NoError;
}

void CTransportStreamParser::close_input_file()
{
	if (m_in_file_stream.is_open())
	{
		m_in_file_stream.close();
	}
}

