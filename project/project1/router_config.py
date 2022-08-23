import redis
import click

@click.group()
# @click.option('--input', nargs=2, type=str)
# @click.option('--source_ip', type=str)
def cli():
    pass

pool = redis.ConnectionPool(host='localhost', db = 0, port=6379, decode_responses=True)
r = redis.Redis(connection_pool=pool)
pipe = r.pipeline() # 创建一个管道


@cli.command()
@click.option('--input', nargs=5, type=str)
def new(input):
    """Simple program that greets NAME for a total of COUNT times."""
    source_ip = input[0]
    route = list(input)
    name = ["source_ip", "source_port", "destination_ip", "destination_port", "rule"]#源IP地址，源端口，目的IP地址，目的端口，传输层协议
    dict = zip(name, route)
    for key, value in dict:
        r.hset("ip:"+str(source_ip), key, str(value))#以源ip作为匹配字段
    pipe.execute()

@cli.command()
@click.option('--source_ip', type=str)
def check(source_ip):
    print(r.hgetall("ip:"+str(source_ip)))

@cli.command()
@click.option('--source_ip', type=str)
def delete(source_ip):
    r.delete("ip:"+str(source_ip))

if __name__ == '__main__':
    cli()