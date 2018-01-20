
import time
import unittest

ONE_WEEK_IN_SECONDS = 7 * 86400
VOTE_SCORE = 432


def article_vote(conn, user, article):
    # 检查文章是否还可以投票
    cutoff = time.time() - ONE_WEEK_IN_SECONDS
    if conn.zscore('time:', article) < cutoff:
        return
    # 按：分割字符串，取最后一个元素为文章ID
    article_id = article.partition(':')[-1]
    if conn.sadd('voted:' + article_id, user):
        conn.zincrby('score:', article, VOTE_SCORE)
        conn.hincrby(article, 'votes', 1)


def post_article(conn, user, title, link):
    article_id = str(conn.incr('article:'))  # A

    voted = 'voted:' + article_id
    conn.sadd(voted, user)  # B
    conn.expire(voted, ONE_WEEK_IN_SECONDS)  # B

    now = time.time()
    article = 'article:' + article_id
    conn.hmset(article, {  # C
        'title': title,  # C
        'link': link,  # C
        'poster': user,  # C
        'time': now,  # C
        'votes': 1,  # C
    })  # C

    conn.zadd('score:', article, now + VOTE_SCORE)  # D
    conn.zadd('time:', article, now)  # D

    return article_id
# <end id="post-article-code"/>
# A Generate a new article id
# B Start with the posting user having voted for the article, and set the article voting information to automatically expire in a week (we discuss expiration in chapter 3)
# C Create the article hash
# D Add the article to the time and score ordered zsets
# END


# <start id="fetch-articles-code"/>
ARTICLES_PER_PAGE = 25


def get_articles(conn, page, order='score:'):
    start = (page - 1) * ARTICLES_PER_PAGE  # A
    end = start + ARTICLES_PER_PAGE - 1  # A

    ids = conn.zrevrange(order, start, end)  # B
    articles = []
    for id in ids:  # C
        article_data = conn.hgetall(id)  # C
        article_data['id'] = id  # C
        articles.append(article_data)  # C

    return articles
# <end id="fetch-articles-code"/>
# A Set up the start and end indexes for fetching the articles
# B Fetch the article ids
# C Get the article information from the list of article ids
# END

# <start id="add-remove-groups"/>


def add_remove_groups(conn, article_id, to_add=[], to_remove=[]):
    article = 'article:' + article_id  # A
    for group in to_add:
        conn.sadd('group:' + group, article)  # B
    for group in to_remove:
        conn.srem('group:' + group, article)  # C
# <end id="add-remove-groups"/>
# A Construct the article information like we did in post_article
# B Add the article to groups that it should be a part of
# C Remove the article from groups that it should be removed from
# END

# <start id="fetch-articles-group"/>


def get_group_articles(conn, group, page, order='score:'):
    key = order + group  # A
    if not conn.exists(key):  # B
        conn.zinterstore(key,  # C
                         ['group:' + group, order],  # C
                         aggregate='max',  # C
                         )
        conn.expire(key, 60)  # D
    return get_articles(conn, page, key)  # E
# <end id="fetch-articles-group"/>
# A Create a key for each group and each sort order
# B If we haven't sorted these articles recently, we should sort them
# C Actually sort the articles in the group based on score or recency
# D Tell Redis to automatically expire the ZSET in 60 seconds
# E Call our earlier get_articles() function to handle pagination and article data fetching
# END

#--------------- Below this line are helpers to test the code ----------------


class TestCh01(unittest.TestCase):
    def setUp(self):
        import redis
        self.conn = redis.Redis(db=15)

    def tearDown(self):
        del self.conn
        print
        print

    def test_article_functionality(self):
        conn = self.conn
        import pprint

        article_id = str(post_article(conn, 'username',
                                      'A title', 'http://www.google.com'))
        print "We posted a new article with id:", article_id
        print
        self.assertTrue(article_id)

        print "Its HASH looks like:"
        r = conn.hgetall('article:' + article_id)
        print r
        print
        self.assertTrue(r)

        article_vote(conn, 'other_user', 'article:' + article_id)
        print "We voted for the article, it now has votes:",
        v = int(conn.hget('article:' + article_id, 'votes'))
        print v
        print
        self.assertTrue(v > 1)

        print "The currently highest-scoring articles are:"
        articles = get_articles(conn, 1)
        pprint.pprint(articles)
        print

        self.assertTrue(len(articles) >= 1)

        add_remove_groups(conn, article_id, ['new-group'])
        print "We added the article to a new group, other articles include:"
        articles = get_group_articles(conn, 'new-group', 1)
        pprint.pprint(articles)
        print
        self.assertTrue(len(articles) >= 1)

        to_del = (
            conn.keys('time:*') + conn.keys('voted:*') + conn.keys('score:*') +
            conn.keys('article:*') + conn.keys('group:*')
        )
        if to_del:
            conn.delete(*to_del)


if __name__ == '__main__':
    unittest.main()
