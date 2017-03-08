


--------------------gcc  g++---------------------------------------------------------
yum install gcc
yum install gcc-c++

yum install jemalloc
yum install tcl

--------------------openresty----------------------------------------------------------

yum install readline-devel pcre-devel openssl-devel
wget https://openresty.org/download/openresty-1.11.2.1.tar.gz
tar -xzvf openresty-VERSION.tar.gz
cd openresty-VERSION/

./configure --prefix=/apps/service/openresty \
            --with-luajit \
            --without-http_redis2_module \
            --with-http_iconv_module

make
make install

--------------------yum安装mysql----------------------------------------------------------
1. 安装mysql 服务器端：
          yum install mysql-server
          yum install mysql-devel
2. 安装mysql客户端：
          yum install mysql
3. 启动mysql服务：
          service mysqld start或者/etc/init.d/mysqld start
停止：
          service mysqld stop
重启：
          service mysqld restart
4. 创建root管理员：
          
#######配置mysql（设置密码等）
[root@typecodes ~]# mysql_secure_installation
NOTE: RUNNING ALL PARTS OF THIS SCRIPT IS RECOMMENDED FOR ALL MySQL
  SERVERS IN PRODUCTION USE! PLEASE READ EACH STEP CAREFULLY!

In order to log into MySQL to secure it, we'll need the current
password for the root user. If you've just installed MySQL, and
you haven't set the root password yet, the password will be blank,
so you should just press enter here.

Enter current password for root (enter for none):
OK, successfully used password, moving on...

Setting the root password ensures that nobody can log into the MySQL
root user without the proper authorisation.


Set root password? [Y/n] y     [设置root用户密码]
New password:
Re-enter new password:
Password updated successfully!
Reloading privilege tables..
 ... Success!


By default, a MySQL installation has an anonymous user, allowing anyone
to log into MySQL without having to have a user account created for
them. This is intended only for testing, and to make the installation
go a bit smoother. You should remove them before moving into a
production environment.

Remove anonymous users? [Y/n] y     [删除匿名用户]
 ... Success!

Normally, root should only be allowed to connect from 'localhost'. This
ensures that someone cannot guess at the root password from the network.

Disallow root login remotely? [Y/n] y  [禁止root远程登录]
 ... Success!

By default, MySQL comes with a database named 'test' that anyone can
access. This is also intended only for testing, and should be removed
before moving into a production environment.

Remove test database and access to it? [Y/n] y   [删除test数据库]
 - Dropping test database...
ERROR 1008 (HY000) at line 1: Can't drop database 'test'; database doesn't exist
 ... Failed! Not critical, keep moving...
 - Removing privileges on test database...
 ... Success!

Reloading the privilege tables will ensure that all changes made so far
will take effect immediately.

Reload privilege tables now? [Y/n] y   [刷新权限]
 ... Success!

All done! If you've completed all of the above steps, your MySQL
installation should now be secure.
Thanks for using MySQL!
Cleaning up...


--------------------redis----------------------------------------------------------

$ wget http://download.redis.io/releases/redis-3.2.4.tar.gz
$ tar xzf redis-3.2.4.tar.gz
$ cd redis-3.2.4
$ make
make install 这个会默认cp到/usr/local/bin ，手动把redis.conf拷贝过去，并修改redis.conf的 daemonize .启动redis  ./redis-server redis.conf redis就以守护模式运行了。


--------------------源码mysql 5.6----------------------------------------------------------
安装MySQL
    yum -y install make gcc-c++ cmake bison-devel  ncurses-devel
    yum -y install autoconf
    wget http://cdn.mysql.com/Downloads/MySQL-5.6/mysql-5.6.20.tar.gz
    tar xvf mysql-5.6.20.tar.gz
    cd mysql-5.6.20

    cmake \
    -DCMAKE_INSTALL_PREFIX=/usr/local/mysql \
    -DMYSQL_DATADIR=/mysql/data \
    -DSYSCONFDIR=/etc \
    -DWITH_MYISAM_STORAGE_ENGINE=1 \
    -DWITH_INNOBASE_STORAGE_ENGINE=1 \
    -DWITH_MEMORY_STORAGE_ENGINE=1 \
    -DWITH_ARCHIVE_STORAGE_ENGINE=1 \
    -DWITH_PARTITION_STORAGE_ENGINE=1 \
    -DWITH_BLACKHOLE_STORAGE_ENGINE=1 \
    -DWITH_READLINE=1 \
    -DENABLE_DOWNLOADS=1 \
    -DENABLED_LOCAL_INFILE=1 \
    -DMYSQL_UNIX_ADDR=/tmp/mysql.sock \
    -DMYSQL_TCP_PORT=3306 \
    -DEXTRA_CHARSETS=all \
    -DDEFAULT_CHARSET=utf8 \
    -DDEFAULT_COLLATION=utf8_general_ci

    make && make install

配置MySQL
    设置权限
    使用下面的命令查看是否有mysql用户及用户组

    cat /etc/passwd 查看用户列表
    cat /etc/group  查看用户组列表如果没有就创建

    groupadd mysql
    useradd -g mysql mysql                  修改/usr/local/mysql权限
    chown -R mysql:mysql /usr/local/mysql   修改/usr/local/mysql权限

    cd /usr/local/mysql
    ./scripts/mysql_install_db --basedir=/usr/local/mysql --datadir=/mysql/data --user=mysql


cp support-files/mysql.server /etc/init.d/mysql
chkconfig mysql on
service mysql start



vim /etc/profile
export PATH=/usr/local/mysql/bin:$PATH
source /etc/profile

执行下面的命令修改root密码
mysql -uroot  
mysql> SET PASSWORD = PASSWORD('123456');  //密码就是123456


--------------------rpm  安装jdk--------------------------------------------------------
chmod 755 jdk-8u77-linux-x64.rpm
rpm   -i  jdk-8u77-linux-x64.rpm

可选{
    vim /etc/profile
        export JAVA_HOME=/usr/java/jdk1.8.0_77
        export JAVA_JRE=/usr/java/jdk1.8.0_77/jre
        export CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar:$JRE_HOME/lib
        export PATH=$PATH:$JAVA_HOME/bin
    source /etc/profile
}
--------------------tomcat7----------------------------------------------------------
http://tomcat.apache.org/
wget http://mirrors.hust.edu.cn/apache/tomcat/tomcat-7/v7.0.73/bin/apache-tomcat-7.0.73.tar.gz
tar xzf apache-tomcat-7.0.73.tar.gz
mv apache-tomcat-7.0.73 /usr/local/tomcat7
bin/catalina.sh start

可选{
    CATALINA_BASE=/usr/local/tomcat7
    CATALINE_HOME=/usr/local/tomcat7
    export CATALINA_BASE CATALINA_HOME
}

--------------------redis----------------------------------------------------------
wget http://download.redis.io/releases/redis-3.2.6.tar.gz
tar xzf redis-3.2.6.tar.gz
cd redis-3.2.6
make
拷贝主要的文件到特定目录
---------------------golang---------------------------------------------------------
为了构建 Go 1.x （x ≥ 5），需要先安装 Go 1.4 到 $GOROOT_BOOTSTRAP.（因为Go 1.5以后 将使用 Go 1.4 进行构建，）
第一步：先下载1.4源码和1.7源码
第二步：cd到go1.4/src里，执行.all.bash...........安装完毕
第三步：export GOROOT_BOOTSTRAP=...go1.4
第四步：cd到go1.7/src里，执行.all.bash...........安装完毕
第五步：设置go1.7的环境
{
export GOROOT=/usr/local/go
export PATH=$PATH:$GOROOT/bin
export GOARCH=amd64
export GOOS=linux
}
--------------------rz命令和sz命令安装----------------------------------------------------------
yum install lrzsz -y
rz
sz filename

------------------------------------------------------------------------------
------------------------------------------------------------------------------
------------------------------------------------------------------------------
------------------------------------------------------------------------------
------------------------------------------------------------------------------
------------------------------------------------------------------------------
------------------------------------------------------------------------------
------------------------------------------------------------------------------
------------------------------------------------------------------------------
------------------------------------------------------------------------------
------------------------------------------------------------------------------
------------------------------------------------------------------------------
------------------------------------------------------------------------------












