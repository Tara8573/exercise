FROM centos:7

MAINTAINER    pkk

RUN echo "create /opt/app"

RUN mkdir /opt/app

RUN echo "add project"

COPY . /opt/app/

WORKDIR /opt/app

RUN cp libhiredis.so /usr/local/lib

RUN  mkdir /usr/local/include/hiredis

RUN cp hiredis.h /usr/local/include/hiredis

RUN echo '/usr/local/lib' >>/etc/ld.so.conf

RUN ldconfig

# CMD ["run your application"]