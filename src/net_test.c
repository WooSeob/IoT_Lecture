#include<stdio.h>
#include<curl/curl.h>
// - - - - - - - - - - - - - - - - - - - -

enum {
	ERROR_ARGS = 1 ,
	ERROR_CURL_INIT = 2
} ;

enum {
	OPTION_FALSE = 0 ,
	OPTION_TRUE = 1
} ;

enum {
	FLAG_DEFAULT = 0 
} ;

// - - - - - - - - - - - - - - - - - - - -

int main(void){

    //Anseong 1 dong wheather information url
    //http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=4155051000

	const char* url = "http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=4155051000" ;

	// lubcURL 초기화 
	curl_global_init( CURL_GLOBAL_ALL ) ;

	// context객체의 생성
	CURL* ctx = curl_easy_init() ;

	if( NULL == ctx ){
		printf("Unable to initialize cURL interface\n");
		return( ERROR_CURL_INIT ) ;
	}

	// context 객체를 설정한다.	
	// 긁어올 url을 명시하고, url이 URL정보임을 알려준다.
	curl_easy_setopt( ctx , CURLOPT_URL,  url ) ;

	// no progress bar:
	curl_easy_setopt( ctx , CURLOPT_NOPROGRESS , OPTION_TRUE ) ;

	/*
	By default, headers are stripped from the output.
	They can be:

	- passed through a separate FILE* (CURLOPT_WRITEHEADER)

	- included in the body's output (CURLOPT_HEADER -> nonzero value)
		(here, the headers will be passed to whatever function
		 processes the body, along w/ the body)

	- handled with separate callbacks (CURLOPT_HEADERFUNCTION)
		(in this case, set CURLOPT_WRITEHEADER to a
		 matching struct for the function)

	*/
	
	// 헤더는 표준에러로 출력하도록 하다. 
	curl_easy_setopt( ctx , CURLOPT_WRITEHEADER , stderr ) ;


	// body 데이터는 표준출력 하도록 한다.
	curl_easy_setopt( ctx , CURLOPT_WRITEDATA , stdout ) ;

	// context 객체의 설정 종료 


	// 웹페이지를 긁어온다. 

	const CURLcode rc = curl_easy_perform( ctx ) ;

	if( rc != CURLE_OK ){
        printf("Error from cURL\n");
	}



	// cleanup
	curl_easy_cleanup( ctx ) ;
	curl_global_cleanup() ;

	return 0;
} // main()