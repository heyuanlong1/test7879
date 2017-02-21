upstream  vsopen.cmgame.com  {
    server    10.141.141.3:80;
}

server {
    listen   80;
    server_name  vsopen.cmgame.com;
    access_log  /var/log/nginx/vsopen.cmgame.com.access.log;
    error_log  /var/log/nginx/vsopen.cmgame.com.error.log;

    root /data/matchvs.com/cms/htdocs/;
    index index.html index.htm;

    location / {
        expires            off;
        proxy_pass         http://vsopen.cmgame.com/;
        proxy_set_header   Host             $host;
        proxy_set_header   X-Real-IP        $remote_addr;
        proxy_set_header   X-Forwarded-For  $proxy_add_x_forwarded_for;
	add_header From    vsopen.cmgame.com;
	proxy_set_header   Cookie $http_cookie;
    }
    

} 
