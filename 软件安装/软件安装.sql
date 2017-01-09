--------------------openresty----------------------------------------------------------

yum install readline-devel pcre-devel openssl-devel
wget https://openresty.org/download/openresty-1.11.2.1.tar.gz
tar -xzvf openresty-VERSION.tar.gz
cd openresty-VERSION/

./configure --prefix=/usr/local/openresty \
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
    useradd -g mysql mysql修改/usr/local/mysql权限

    chown -R mysql:mysql /usr/local/mysql修改/usr/local/mysql权限

    cd /usr/local/mysql
    scripts/mysql_install_db --basedir=/usr/local/mysql --datadir=/mysql/data --user=mysql


cp support-files/mysql.server /etc/init.d/mysql
chkconfig mysql on
service mysql start



vim /etc/profile
export PATH=/usr/local/mysql/bin:$PATH
source /etc/profile

执行下面的命令修改root密码
mysql -uroot  
mysql> SET PASSWORD = PASSWORD('123456');  //密码就是123456


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
------------------------------------------------------------------------------
------------------------------------------------------------------------------
------------------------------------------------------------------------------
------------------------------------------------------------------------------












