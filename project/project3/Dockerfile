FROM redis:4.0.6

WORKDIR /opt/redis

COPY myredis.conf /opt/redis/

RUN pip install ping3 && pip install schedule && pip install redis

CMD sh -c "python3 dt.py"
