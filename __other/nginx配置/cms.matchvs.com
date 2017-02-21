upstream  testcms2.matchvs.com  {
    server    127.0.0.1:8080;
}

server {
    listen   80;
    server_name  testcms2.matchvs.com;
    access_log  /var/log/nginx/testcms2.matchvs.com.access.log;
    error_log  /var/log/nginx/testcms2.matchvs.com.error.log;

    root /data/matchvs.com/cms/htdocs/;
    index index.html index.htm;

    location / {
        expires            off;
        proxy_pass         http://testcms2.matchvs.com/admin/;
        proxy_set_header   Host             $host;
        proxy_set_header   X-Real-IP        $remote_addr;
        proxy_set_header   X-Forwarded-For  $proxy_add_x_forwarded_for;
	add_header From    testcms2.matchvs.com;
	proxy_cookie_path  /admin/ /;
	proxy_set_header   Cookie $http_cookie;
    }
    
   location ^~ /admin/ {
        expires            off;
        proxy_pass         http://testcms2.matchvs.com/admin/;
        proxy_set_header   Host             $host;
        proxy_set_header   X-Real-IP        $remote_addr;
        proxy_set_header   X-Forwarded-For  $proxy_add_x_forwarded_for;
	add_header From    testcms2.matchvs.com;
	proxy_cookie_path  /admin/ /;
	proxy_set_header   Cookie $http_cookie;
   }


} 
