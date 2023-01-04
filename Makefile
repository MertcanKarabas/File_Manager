all: file_manager file_client

file_manager: file_manager.c
	gcc file_manager.c -o file_manager

file_client: file_client.c
	gcc file_client.c -o file_client

clean:
	rm -rf *o file_manager
	rm -rf *o file_client