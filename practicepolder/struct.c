#include <stdio.h>
#include <string.h>
#include <stdint.h>
uint8_t at_data[256];
// ����ü ����
typedef struct data_format {
    char* name;			/* Command Name */
    int lmin;			/* minimum abbreviated length */
    int maxargs;		/* maximum number of arguments */
    int repeatable;		/* autorepeat allowed? */
    int (*fun) (struct data_format*, int, char* [], int);	/* implementation function */
}data_format_t;

// �Լ� ����
int cmd_sum(data_format_t* cmddata, int argc, char* argv[], int repeat);
int cmd_imsum(data_format_t* cmddata, int argc, char* argv[], int repeat);

//��ũ�� ����
#define MK_CMD_TBL_ENTRY(name,lmin,maxargs,rep,cmd) \
	{ name, lmin, maxargs, rep, cmd}

// mk_cmd_tbl_entry  ��ũ�θ� �̿��� ������ �Է�
#define CMD_TBL_SUM MK_CMD_TBL_ENTRY("sum", 3, 3, 0, cmd_sum)
#define CMD_TBL_IMSUM MK_CMD_TBL_ENTRY("imsum", 3, 3, 0, cmd_imsum)

// ����ü ���� ����
data_format_t cmd_tbl[] = {
    CMD_TBL_SUM,
    CMD_TBL_IMSUM
};

//sum, imsum �Լ� ����
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
//cmd�� �Է� ���� ���� �и��Ͽ� �����ϴ� �Լ�.
int parse_cmd(char* line, char* argv[]) // line : �Է¹��� ���ڿ�, argv : �и��� ���ڿ�
{
    memset(at_data, 0, sizeof(at_data)); // at_data �ʱ�ȭ
    //memcpy(at_data, line, sizeof(line)); // at_data�� line ����
    int idx = 0;
    char* next;
    char* token;

    token = strtok_s(line, " ", &next);  // ù ��° ��ū ����
    while (idx < 20) {
        argv[idx] = token; // argv�� ��ū ����
        token = strtok_s(NULL, " ",&next); // ���� ��ū ����
        idx++;
        //printf("token: %c",token);
    }
    return idx; // ��ū ���� ��ȯ
}



// �Է� cmd�� �ش��ϴ� �Լ� ȣ��
data_format_t* find_cmd(const char* cmd) { // data_format_t* �� ��ȯ���� ����ü �ּ��ε� int �̷��� �����ϸ� ����ü�� ũ�Ⱑ �� ����.
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
    char* argv[20];  // �и��� ��ɾ�� ���ڸ� ������ �迭
    int argc;  // ������ ����

    printf("Enter a command: ");
    fgets(line, sizeof(line), stdin);  // ����ڷκ��� �Է� �ޱ�

    // ���� ���� ����
    line[strcspn(line, "\n")] = 0;

    // ��ɾ�� ���� �и�
    argc = parse_cmd(line, argv);

    // ��ɾ� �˻�
    data_format_t* cmdtp = find_cmd(argv[0]);

    if (cmdtp == NULL) {
        printf("Unknown command '%s' - try 'help'\n", argv[0]);
    }
    else {
        // ��ɾ �ش��ϴ� �Լ� ����
        cmdtp->fun(cmdtp, argc, argv, 0);  // �Լ� ȣ��
    }

    return 0;
}

