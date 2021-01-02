#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/timerfd.h>
#include <sys/user.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <netpacket/packet.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <sys/un.h>
#include <sys/eventfd.h>
#include "sys/types.h"

unsigned char BROADCAST_ADDR[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
unsigned char STATION_ADDR[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

struct eth_header {
	unsigned char destaddr[6];
	unsigned char srcaddr[6];
	unsigned short etherType;
};

struct myarp_header {
	unsigned int len;
	unsigned char ethAddr[6];
	unsigned char data[0];
};

struct registered_dst {
	char* id;				// virtual machine identifier
	unsigned char dst[6];	// the vm MAC address 
	struct registered_dst* next;
};

int sock_ll;
char* interface;
int init_socket(unsigned short ethr_type, int* sock_ll_ptr);
struct registered_dst* waitARPReply(char* vm_id);
void sendARPRequest(char* id);
void sendFrameTo(char* id, unsigned short ethr_type, unsigned char* data, int len);
void sendFrame(unsigned char* dst_mac, unsigned short ether_type, unsigned char* data, int len);
struct registered_dst* findEntry(char* id);
void dispatchReceivedFrame(unsigned char* buff_ptr);
void receiveARPFrame(unsigned char* dst_mac, unsigned char* arp_packet);
void receiveDataFrame(unsigned char* dst_mac, unsigned char* data_packet);


unsigned char* name;	// my vm name
struct registered_dst dst_list; // arp cache list


int main(int argc, char* argv[]) {
	char* target;
	unsigned char buffer[1500];
	unsigned int received = 0;

	interface = strdup(argv[1]);	// the interface name on my vm, ex.: "ethx" "enpXsX"
	name = strdup(argv[2]);			// the my vm name, ex.: "VMx" 
	target = strdup(argv[3]);		// the peer vm name, ex.: "VMy"

	dst_list.next = NULL;
	if (init_socket(ETH_P_ALL, &sock_ll) == -1) {
		printf("Socket Error - Terminate Program\n");
		return 0;
	}

	/* !!!! VM1 main code !!!! */
	//sendFrameTo(target, 0xFFFD, "Hi Hello AnnyeongHaSeYo !!!!!!!2019320061!!!!!!!", strlen("Hi Hello AnnyeongHaSeYo !!!!!!!2019320061!!!!!!!")); // When you write your student code, delete !!!!!!!!

	//received = recv(sock_ll, buffer, 1500, 0);
	//dispatchReceivedFrame(buffer);
	//sendFrameTo(target, 0xFFFD, "I'm Glad to meet you again.", strlen("I'm Glad to meet you again."));

	//received = recv(sock_ll, buffer, 1500, 0);
	//dispatchReceivedFrame(buffer);


	/* !!!! VM2 main code !!!! */
	 while (1) {
		received = recv(sock_ll, buffer, 1500, 0);
		dispatchReceivedFrame(buffer);
	 }

	close(sock_ll);
	return 0;
}

void dispatchReceivedFrame(unsigned char* ptr) {
	struct eth_header* eh;
	unsigned short receivedEtherType;

	eh = (struct eth_header*)ptr;
	ptr += sizeof(struct eth_header);

	receivedEtherType = ntohs(eh->etherType);

	switch (receivedEtherType) {
	case 0xFFFE:
		receiveARPFrame(eh->srcaddr, ptr);
		break;
	case 0xFFFD:
		receiveDataFrame(eh->srcaddr, ptr);
		break;
	default:
		break;
	}
}

void receiveARPFrame(unsigned char* dst, unsigned char* arp) {
	struct myarp_header* ra;
	struct myarp_header* mah;
	unsigned char* receivedId;
	unsigned int size = sizeof(struct myarp_header) + strlen(name);
	ra = (struct myarp_header*)arp;

	printf("ARP Received from %x:%x:%x:%x:%x:%x.\n", dst[0], dst[1], dst[2], dst[3], dst[4], dst[5]);

	// ! Allocate memory for receivedId. Size is received identifier length + 1. !
	receivedId = (unsigned char*)malloc(strlen(ra->data));
	// ! Get ReceivedID from received packets !
	memcpy(receivedId, ra->data, (sizeof(receivedId)));
	// ! When you get the receivedId, ID String must have NULL value at last for proper operation of strcmp. !
	//receivedId[strlen(receivedId)] = 0;

	if (strcmp(name, receivedId)) {
		printf("Received Target name is not mine. ignore..\n");
		return;
	}

	mah = (struct myarp_header*)malloc(size);
	// ! Make ARP Reply. Fill identifier length, identifier and the ethernet_address field of ARP Reply with its MAC Address(STATION_ADDR). !
	memcpy(mah->data, name, sizeof(name));
	memcpy(mah->ethAddr, STATION_ADDR, sizeof(STATION_ADDR));
	mah->len = size;

	sendFrame(dst, 0xFFFE, (unsigned char*)mah, size);
}

void receiveDataFrame(unsigned char* dst, unsigned char* data) {
	printf("Received Data : %s\n", data);
	/* !!!! VM2 main code !!!! */
	sendFrame(dst, 0xFFFD, "Nice to meet you!\n", strlen("Nice to meet you!\n"));
}

struct registered_dst* findEntry(char* id) {
	struct registered_dst* ptr;
	for (ptr = dst_list.next; ptr != NULL; ptr = ptr->next) {
		if (!strcmp(id, ptr->id)) {
			printf("Finding Entry Success\n");
			return ptr;
		}
	}

	printf("Failed to find entry.\n");
	return NULL;
}

void sendARPRequest(char* id) { // Make ARP Contents and call sendFrame
	struct myarp_header* mah;
	int size = sizeof(struct myarp_header) + strlen(id);
	mah = (struct myarp_header*)malloc(size); // Allocate memory for ARP Contents

	// ! Fill the mah with ARP Contents (Identifier, DST is all 0, identifier length..) !
	memcpy(mah->data, id, sizeof(id));
	memset((mah->ethAddr), 0, sizeof(mah->ethAddr));
	mah->len = size;

	sendFrame(BROADCAST_ADDR, 0xFFFE, (unsigned char*)mah, size);
}

struct registered_dst* waitARPReply(char* id) {
	int received;
	unsigned char buffer[1500];
	char* receivedId;
	struct eth_header* eh;
	struct myarp_header* mah;

	while (1) {
		received = recv(sock_ll, buffer, 1500, 0);
		eh = (struct eth_header*)buffer;
		if (eh->etherType != ntohs(0xFFFE))
			continue;

		mah = (struct myarp_header*)(buffer + sizeof(struct eth_header));
		// mah is pointing arp contents starting pointer.
		
		// ! Allocate memory for receivedId. Size is received identifier length + 1. !
		receivedId = (char*)malloc(strlen(mah->data));
		// ! Parse received arp contents (receivedId, ethernet_address of peer, identifier length..) !
		memcpy(receivedId, mah->data, sizeof(receivedId));
		// ! When you get the receivedId, ID String must have NULL value at last for proper operation of strcmp. !
		//receivedId[strlen(receivedId)] = 0;

		if (!strcmp(receivedId, id)) {
			struct registered_dst* ptr;
			struct registered_dst* newEntry;

			printf("ARP Received. Target MAC Address is %x:%x:%x:%x:%x:%x\n", mah->ethAddr[0], mah->ethAddr[1], mah->ethAddr[2], mah->ethAddr[3], mah->ethAddr[4], mah->ethAddr[5]);

			for (ptr = &dst_list; ptr->next != NULL; ptr = ptr->next) { // Go to last entry of arp cache
			}
			
			newEntry = (struct registered_dst*)malloc(sizeof(struct registered_dst));
			// ! Make newEntry using parsed ARP contents and link it to arp cache !
			ptr -> next = newEntry;
			newEntry->id = malloc(strlen(id) + 1);
			memcpy(newEntry->dst, mah->ethAddr, sizeof(newEntry->dst));
			memcpy(newEntry->id, mah->data, sizeof(newEntry->id));
			newEntry->next = NULL;

			return newEntry;
		}
	}
	return NULL;
}

void sendFrameTo(char* id, unsigned short type, unsigned char* data, int len) {
	struct registered_dst* entry = findEntry(id);
	printf("Sending Message.. : %s\n", data);
	if (entry == NULL) {
		printf("Send ARP..\n");
		sendARPRequest(id);
		entry = waitARPReply(id);
	}

	sendFrame(entry->dst, type, data, len);
}

void sendFrame(unsigned char* dst, unsigned short type, unsigned char* data, int len) {
	unsigned char* msgbuf, * msgbuf_wrptr;
	int msgLength = 0;
	int bytes = 0;
	struct eth_header* eh;

	msgbuf = (unsigned char*)malloc(2000);
	if (msgbuf == NULL) {
		return;
	}

	memset(msgbuf, 0, 2000);
	msgbuf_wrptr = msgbuf;

	eh = (struct eth_header*)msgbuf_wrptr;

	// ! Build ethernet header part of frame and frame payload. !
	memcpy(eh->destaddr, dst, sizeof(dst));
	memcpy(eh->srcaddr, STATION_ADDR, sizeof(STATION_ADDR));
	eh->etherType = ntohs(type);	//htons? ntohs?
	
	msgbuf_wrptr += sizeof(struct eth_header);
	memcpy(msgbuf_wrptr, data, len);
	msgbuf_wrptr += len;

	bytes = send(sock_ll, msgbuf, (int)(msgbuf_wrptr - msgbuf), 0);
	free(msgbuf);
}

int init_socket(unsigned short etype, int* sock) { // Initialize socket for L2.
	struct sockaddr_ll addr;
	struct ifreq if_request;
	int lsock;
	int rc;
	struct packet_mreq multicast_req;

	if (NULL == sock)
		return -1;

	*sock = -1;

	lsock = socket(PF_PACKET, SOCK_RAW, htons(etype));

	if (lsock < 0) {
		printf("Socket Creation Error\n");
		return -1;
	}

	memset(&if_request, 0, sizeof(if_request));

	strncpy(if_request.ifr_name, interface, sizeof(if_request.ifr_name) - 1);


	rc = ioctl(lsock, SIOCGIFHWADDR, &if_request);
	if (rc < 0) {
		printf("IOCTL Error\n");
		close(lsock);
		return -1;
	}

	memcpy(STATION_ADDR, if_request.ifr_hwaddr.sa_data,
		sizeof(STATION_ADDR));

	memset(&if_request, 0, sizeof(if_request));

	strncpy(if_request.ifr_name, interface, sizeof(if_request.ifr_name) - 1);

	rc = ioctl(lsock, SIOCGIFINDEX, &if_request);
	if (rc < 0) {
		printf("IOCTL2 Error\n");
		close(lsock);
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sll_ifindex = if_request.ifr_ifindex;
	addr.sll_family = AF_PACKET;
	addr.sll_protocol = htons(etype);

	rc = bind(lsock, (struct sockaddr*)&addr, sizeof(addr));
	if (0 != rc) {
#if LOG_ERRORS
		fprintf(stderr, "%s - Error on bind %s", __FUNCTION__, strerror(errno));
#endif
		close(lsock);
		return -1;
	}

	rc = setsockopt(lsock, SOL_SOCKET, SO_BINDTODEVICE, interface,
		strlen(interface));
	if (0 != rc) {
		printf("Bind option error\n");
		close(lsock);
		return -1;
	}

	*sock = lsock;

	return 0;
}
