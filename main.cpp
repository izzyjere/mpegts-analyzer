#include <stdio.h>
#include "TSParserLib.h"
void printMenu()
{
	cout<<"====== ET591 ASSIGNMENT =======\n";
	cout<<"\n[1] Print nth Transport Stream Packet\n[2] Display the last nth bytes\n[3] Display File Size\n[4] Dump File Info\n[0] Exit\n";	
}
void printHelp()
{
	//to be implemented
}
int main(int argc, char **argv)
{    
	//File name should be passed as argument
	if(argv[1] == NULL)
	{
		cout<<"Please pass the file name as argument\n";
		return -1;
	}
	CTransportStreamParser ts_parser(argv[1]);
	int choice = 0;
	do
	{
		printMenu();
		cout<<"Select Option: ";
		cin>>choice;
		switch(choice)
		{
			case 0:
			   cout<<"Exiting... Goodbye!"<<endl;
			   break;
			case 1:
			{
				int index = 0;
				cout<<"Enter Packet position: ";
				cin>>index;
				ts_parser.Dump_ts_packet(index);
				 break;
			}			
			case 2:
			{
				int numberOfBytes = 0;
				cout<<"Enter number of bytes to read: ";
				cin>>numberOfBytes;
				ts_parser.Dump_ts_bytes(numberOfBytes,true);
				break;
			}		
			case 3:
			//print file name and size
			ts_parser.Dump_ts_info();
			break;		
			case 4:
			{
				int index = 0;
				cout<<"Enter Packet position: ";
				cin>>index;
				//Print information about a specific packet
				ts_parser.Dump_ts_packet_info(index);
				 break;
			}
			default:
			cout<<"Invalid Input!!";

			
		}

	}while(choice!=0);
	return 0;
}