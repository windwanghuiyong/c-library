int strspn(const char *s,const char *accept)
{
    const char *p;
    const char *a;
    int count = 0;
    for(p = s; *p != '\0'; ++p)
    {
        for (a = accept; *a != '\0'; ++a)
        {
            if (*p == *a)
            {
                ++count;
                break;
            }
        }//里面的for循环到此为止

        if (*a == '\0')
        {
            return count;
        }
        //++count;
    }//外面的for循环到此为止
    return count;
}

char *strstr(char *s1, char *s2)
{
    int n;
    while(*s1)
    {
        for (n = 0; *(s1 + n) == *(s2 + n); n++)
        {
            if (!*(s2 + n + 1))            // s2 的末尾
            {
                return (char*)s1;
            }
        }
        s1++;
    }
    return NULL;
}

char *strtok(char *str_org, const char *delimiter)
{
    static unsigned char	*str_remain;
    unsigned char			*str_scan;
    const unsigned char		*ctrl = (const unsigned char*)delimiter;
	unsigned char			map[32];
    int						count;

    /*将分隔字置入索引表，一个位代表一个字符，32*8=255个字符*/
    /* 初始化 map */
    for (count = 0; count < 32; count++)
        map[count] = 0;
	/* 左侧用当前字符的高5位确定数组偏移，右侧用低3位确定字节中的位偏移 */
    do
    {
        map[*ctrl >> 3] |= (1 << (*ctrl & 7));
    } while (*ctrl++);

    /* 首次调用取原始字符串，随后的调用取剩余字符串(参数设为 NULL) */
    if (str_org)
        str_scan = (unsigned char*)str_org;
    else
        str_scan = str_remain;

    /* 略过当前字符串前面的分隔符 */
    while ((map[*str_scan >> 3] & (1 << (*str_scan & 7)))  && *str_scan)
    {
        str_scan++;
    }

    //重置需要扫描的字符串
    str_org = (char*)str_scan;

    //开始扫描
    for (; *str_scan; str_scan++)
    {
        if ( map[*str_scan >> 3] & (1 << (*str_scan & 7)))
        {
            *str_scan++ = '\0';//当找到时，把匹配字符填为0，并且把str_scan指向下一位。
            break; //退出循环
        }
    }

    str_remain = str_scan; // 把剩余字符串的指针保存到静态变量str_remain中。

    if (str_org == (char*)str_scan)
        return NULL;		//没有找到，也就是没有移动指针的位置，如剩余字符串全是分隔符，返回NULL
    else
        return str_org;		//找到了，返回之前字符串的头指针
}

/* netbsd */
char *strtok_r(char *string_org, const char *demial, char **last)
{
	const char	*span_pointer;
	char		*tok_pointer;
	char		chr, span_chr;

	/* 当前查找的字符串且已保存的字符串为 NULL 则停止查找，将已保存的字符串当作当前查找的字符串 */
	if (string_org == NULL  && (string_org = *last) == NULL)
    	return (NULL);

	/* 跳过分隔符 */
	count:
	chr = *string_org++;
	for (span_pointer = demial; (span_chr = *span_pointer++) != 0; )
    {
	    if (chr == span_chr)
	    	goto count;
    }

	/* 查找到字符串末尾 */
	if (chr == 0)
	{
	    *last = NULL;
	    return (NULL);
	}

	/* 指向找到的字符串的起始位置 */
	tok_pointer = string_org -1;

	//开始扫描字符串中是否含有要匹配的字符，之后把这个匹配字符之前的部分返回。
	//这看似是个无限循环，但当源字符串和匹配字符串都走到结尾时，也就是string_org和span_chr都为NULL时，最外层循环最后会走到return(tok_pointer)结束循环。
	for (;;)
    {
	    chr = *string_org++;
	    span_pointer = demial;

	    do
	    {
			/* 字符串中的当前字符对照每一个分隔符 */
	        if ((span_chr = *span_pointer++) == chr)
	        {
	            if (chr == 0)
	            {
	                string_org = NULL;
	            }
	            else
	            {
	                string_org[-1] = 0;
	            }
	            *last = string_org;
	            return (tok_pointer);
	        }
	    } while (span_chr != 0);
    }
}

/***
*strtok.c - tokenize a string with given delimiters
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines strtok() - breaks string into series of token
*       via repeated calls.
*
*******************************************************************************/
#include
#include
#ifdef _MT
#include
#endif  /* _MT */
/***
*char *strtok(string, control) - tokenize string with delimiter in control
*
*Purpose:
*       strtok considers the string to consist of a sequence of zero or more
*       text tokens separated by spans of one or more control chars. the first
*       call, with string specified, returns a pointer to the first char of the
*       first token, and will write a null char into string immediately
*       following the returned token. subsequent calls with zero for the first
*       argument (string) will work thru the string until no tokens remain. the
*       control string may be different from call to call. when no tokens remain
*       in string a NULL pointer is returned. remember the control chars with a
*       bit map, one bit per ascii char. the null char is always a control char.
*       //这里已经说得很详细了!!比MSDN都好!
*Entry:
*       char *string - string to tokenize, or NULL to get next token
*       char *control - string of characters to use as delimiters
*
*Exit:
*       returns pointer to first token in string, or if string
*       was NULL, to next token
*       returns NULL when no more tokens remain.
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/
char * __cdecl strtok (char * string, const char * control)
{
        unsigned char *str;
        const unsigned char *ctrl = control;
        unsigned char map[32];
        int count;
#ifdef _MT
        _ptiddata ptd = _getptd();
#else  /* _MT */
        static char *nextoken;                        //保存剩余子串的静态变量
#endif  /* _MT */
        /* Clear control map */
        for (count = 0; count < 32; count++)
                map[count] = 0;
        /* Set bits in delimiter table */
        do {
                map[*ctrl >> 3] |= (1 << (*ctrl & 7));
        } while (*ctrl++);
        /* Initialize str. If string is NULL, set str to the saved
         * pointer (i.e., continue breaking tokens out of the string
         * from the last strtok call) */
        if (string)
                str = string;                             //第一次调用函数所用到的原串
else
#ifdef _MT
                str = ptd->_token;
#else  /* _MT */
                str = nextoken;                      //将函数第一参数设置为NULL时调用的余串
#endif  /* _MT */
  /* Find beginning of token (skip over leading delimiters). Note that
         * there is no token iff this loop sets str to point to the terminal
         * null (*str == '\0') */
        while ( (map[*str >> 3] & (1 << (*str & 7))) && *str )
                str++;
        string = str;                                  //此时的string返回余串的执行结果
        /* Find the end of the token. If it is not the end of the string,
         * put a null there. */
//这里就是处理的核心了, 找到分隔符,并将其设置为'\0',当然'\0'也将保存在返回的串中
        for ( ; *str ; str++ )
                if ( map[*str >> 3] & (1 << (*str & 7)) ) {
                        *str++ = '\0';              //这里就相当于修改了串的内容 ①
                        break;
                }
        /* Update nextoken (or the corresponding field in the per-thread data
         * structure */
#ifdef _MT
        ptd->_token = str;
#else  /* _MT */
        nextoken = str;                 //将余串保存在静态变量中,以便下次调用
#endif  /* _MT */
        /* Determine if a token has been found. */
        if ( string == str )
              return NULL;
        else
                return string;
}
