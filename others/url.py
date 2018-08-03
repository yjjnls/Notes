import requests
import time

urls = ['https://blog.csdn.net/yjjnls/article/details/81380786',
        'https://blog.csdn.net/yjjnls/article/details/81352176',
        'https://blog.csdn.net/yjjnls/article/details/81324517',
        'https://blog.csdn.net/yjjnls/article/details/81324451',
        'https://blog.csdn.net/yjjnls/article/details/81324442',
        'https://blog.csdn.net/yjjnls/article/details/81324434',
        'https://blog.csdn.net/yjjnls/article/details/81321528',
        'https://blog.csdn.net/yjjnls/article/details/81298831',
        'https://blog.csdn.net/yjjnls/article/details/81297245',
        'https://blog.csdn.net/yjjnls/article/details/81283840',
        'https://blog.csdn.net/yjjnls/article/details/81283801',
        'https://blog.csdn.net/yjjnls/article/details/81283755',
        'https://blog.csdn.net/yjjnls/article/details/81283731']


while(1):
    for url in urls:
        try:
            print url
            res = requests.get(url)
            print res.text
            time.sleep(3)
        except:
            print "error"
    print ""
    time.sleep(20)
