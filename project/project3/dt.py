from ping3 import ping, verbose_ping
import redis
import schedule

class ScalarMetric(object):
    def __init__(self):
        self.value = 0.0
        self.num_observations = 0.0
        self.aggregated_value = 0.0
        self.reset()

    def reset(self):
        self.value = []
        self.num_observations = 0.0
        self.aggregated_value = 0.0

    def __repr__(self):
        return str(self.peek())

    def update(self, x):
        self.aggregated_value += x
        self.num_observations += 1

    def peek(self):
        return self.aggregated_value / (
            self.num_observations if self.num_observations > 0 else 1
        )

def run():
    pool=redis.ConnectionPool(host='127.0.0.1',port=6379,db=1)
    r = redis.StrictRedis(connection_pool=pool)
    target_redis = redis.Redis(host='127.0.0.1', port=6379, db=2)
    keys = r.keys()
    for key in keys:
        print(key)
        res = ping(key[3:])#14.215.177.39
        SSIM = ScalarMetric()
        if(res):
            SSIM.update(res)
            target_redis.hset(key, "available", "Yes")
            target_redis.hset(key, "Time", float(res))
            target_redis.expire(key, 3600)
        else:
            target_redis.hset(key, "available", "No")
            target_redis.hset(key, "Time", 0.0)
            target_redis.expire(key, 3600)


def main():
    # pool=redis.ConnectionPool(host='127.0.0.1',port=6379,db=1)
    # r = redis.StrictRedis(connection_pool=pool)
 
    # keys = r.keys()
    schedule.every(15).minutes.do(run)    # 每隔十分钟执行一次任务
    # schedule.every(10).seconds.do(run) 
    # for key in keys:
    #     print(key)
    #     res = ping(key[3:])#14.215.177.39
    #     SSIM = ScalarMetric()
    #     if(res):
    #         SSIM.update(res)
    #     print(res)
    while True:
        schedule.run_pending()  # run_pending：运行所有可以运行的任务


if __name__ == '__main__':
    main()