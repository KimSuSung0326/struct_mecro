#include <stdio.h>
#include <string.h>
#include <stdint.h>
uint8_t at_data[256];
// 구조체 선언
typedef struct data_format {
    char* name;			/* Command Name */
    int lmin;			/* minimum abbreviated length */
    int maxargs;		/* maximum number of arguments */
    int repeatable;		/* autorepeat allowed? */
    int (*fun) (struct data_format*, int, char* [], int);	/* implementation function */
}data_format_t;

// 함수 선언
int cmd_sum(data_format_t* cmddata, int argc, char* argv[], int repeat);
int cmd_imsum(data_format_t* cmddata, int argc, char* argv[], int repeat);

//메크로 선언
#define MK_CMD_TBL_ENTRY(name,lmin,maxargs,rep,cmd) \
	{ name, lmin, maxargs, rep, cmd}

// mk_cmd_tbl_entry  매크로를 이용한 데이터 입력
#define CMD_TBL_SUM MK_CMD_TBL_ENTRY("sum", 3, 3, 0, cmd_sum)
#define CMD_TBL_IMSUM MK_CMD_TBL_ENTRY("imsum", 3, 3, 0, cmd_imsum)

// 구조체 변수 선언
data_format_t cmd_tbl[] = {
    CMD_TBL_SUM,
    CMD_TBL_IMSUM
};

//sum, imsum 함수 선언
int cmd_sum(data_format_t* cmddata, int argc, char* argv[], int repeat) {
    uint8_t num[3];
    num[0] = atoi(argv[1]);
    num[1] = atoi(argv[2]);
    num[2] = num[0] + num[1];
    printf("sum %d + %d = %d \n",num[0], num[1],num[2]);
    return 0;
};
int cmd_imsum(data_format_t* cmddata, int argc, char* argv[], int repeat) {
    printf("imsum\n");
    return 0;
};
//cmd로 입력 받은 값을 분리하여 저장하는 함수.
int parse_cmd(char* line, char* argv[]) // line : 입력받은 문자열, argv : 분리된 문자열
{
    memset(at_data, 0, sizeof(at_data)); // at_data 초기화
    //memcpy(at_data, line, sizeof(line)); // at_data에 line 복사
    int idx = 0;
    char* next;
    char* token;

    token = strtok_s(line, " ", &next);  // 첫 번째 토큰 추출
    while (idx < 20) {
        argv[idx] = token; // argv에 토큰 저장
        token = strtok_s(NULL, " ",&next); // 다음 토큰 추출
        idx++;
        //printf("token: %c",token);
    }
    return idx; // 토큰 개수 반환
}



// 입력 cmd에 해당하는 함수 호출
data_format_t* find_cmd(const char* cmd) { // data_format_t* 는 반환값이 구조체 주소인데 int 이렇게 선언하면 구조체의 크기가 안 맞음.
    data_format_t* cmdp;

    for (cmdp = &cmd_tbl[0]; cmdp->name; cmdp++) {
        if (strcmp(cmd, cmdp->name) == 0) {
            return cmdp;
        }
    }
    return NULL;
};

int main() {
    char line[256];
    char* argv[20];  // 분리된 명령어와 인자를 저장할 배열
    int argc;  // 인자의 개수

    printf("Enter a command: ");
    fgets(line, sizeof(line), stdin);  // 사용자로부터 입력 받기

    // 개행 문자 제거
    line[strcspn(line, "\n")] = 0;

    // 명령어와 인자 분리
    argc = parse_cmd(line, argv);

    // 명령어 검색
    data_format_t* cmdtp = find_cmd(argv[0]);

    if (cmdtp == NULL) {
        printf("Unknown command '%s' - try 'help'\n", argv[0]);
    }
    else {
        // 명령어에 해당하는 함수 실행
        cmdtp->fun(cmdtp, argc, argv, 0);  // 함수 호출
    }

    return 0;
}

