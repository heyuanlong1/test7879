
server {
    listen   80;
    server_name  testupload.matchvs.com;
    access_log  /var/log/nginx/testupload.matchvs.com.access.log;
    error_log  /var/log/nginx/testupload.matchvs.com.error.log;

    root /data/matchvs.com/upload/htdocs/;
    index index.html index.htm;


    location /baiduMatch/ {
	root /data/matchvs.com/upload/htdocs/ 	;
    }
	
  location /wc6/uploadAvatar {
     proxy_pass   http://127.0.0.1:9090/open/gc1/uploadAvatar;
     proxy_set_header X-Real-IP $remote_addr;
     proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
   }

	location /wc6/getAvatarInfo {

     proxy_pass   http://127.0.0.1:9090/open/gc1/getAvatarInfo;
     proxy_set_header X-Real-IP $remote_addr;
     proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;


	}

} 
