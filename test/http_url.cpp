#include <gmock/gmock.h>

#include "http/url.h"

using namespace leaf::network;

TEST(HTTP_URL, valid) {
	url url_1("http://www.example.com");
	EXPECT_EQ(url_1.scheme, "http");
	EXPECT_EQ(url_1.host, "www.example.com");
	EXPECT_EQ(url_1.path, "");
	url url_2("https://example.com:79?a20=b39");
	EXPECT_EQ(url_2.scheme, "https");
	EXPECT_EQ(url_2.host, "example.com");
	EXPECT_EQ(url_2.port, 79);
	EXPECT_EQ(url_2.path, "");
	EXPECT_THAT(url_2.query, testing::Contains(testing::Pair("a20", "b39")));
	url url_3("ftp://www.gookle.com.lla:/ppa/aap?df=fd#ddddd");
	EXPECT_EQ(url_3.scheme, "ftp");
	EXPECT_EQ(url_3.host, "www.gookle.com.lla");
	EXPECT_EQ(url_3.path, "/ppa/aap");
	EXPECT_THAT(url_3.query, testing::Contains(testing::Pair("df", "fd")));
	EXPECT_EQ(url_3.fragment, "ddddd");
	url url_4("ftp://w.lla/ppa/aap/#ddddd");
	EXPECT_EQ(url_4.scheme, "ftp");
	EXPECT_EQ(url_4.host, "w.lla");
	EXPECT_EQ(url_4.path, "/ppa/aap/");
	EXPECT_EQ(url_4.fragment, "ddddd");
	url url_5{"urn:example:ffghj:435"};
	EXPECT_EQ(url_5.scheme, "urn");
	EXPECT_EQ(url_5.path, "example:ffghj:435");
}

TEST(HTTP_URL, invalid) {
	EXPECT_THROW(url{"1hts:urk:aldjf?aa"}, std::runtime_error);
	EXPECT_THROW(url{"ht^s:urk:aldjf#45"}, std::runtime_error);
}

int main() {
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}
