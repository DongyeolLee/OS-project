[BITS 64]           ; ������ �ڵ�� 64��Ʈ �ڵ�� ����

SECTION .text       ; text ����(���׸�Ʈ)�� ����

; C ���� ȣ���� �� �ֵ��� �̸��� ������(Export)
global kInPortByte, kOutPortByte, kLoadGDTR, kLoadTR, kLoadIDTR
global kEnableInterrupt, kDisableInterrupt, kReadRFLAGS
global kReadTSC

; ��Ʈ�κ��� 1����Ʈ�� ����
;   PARAM: ��Ʈ ��ȣ
kInPortByte:
    push rdx        ; �Լ����� �ӽ÷� ����ϴ� �������͸� ���ÿ� ����
                    ; �Լ��� ������ �κп��� ���ÿ� ���Ե� ���� ���� ����

    mov rdx, rdi    ; RDX �������Ϳ� �Ķ���� 1(��Ʈ ��ȣ)�� ����
    mov rax, 0      ; RAX �������͸� �ʱ�ȭ
    in al, dx       ; DX �������Ϳ� ����� ��Ʈ ��巹������ �� ����Ʈ�� �о�
                    ; AL �������Ϳ� ����, AL �������ʹ� �Լ��� ��ȯ ������ ����

    pop rdx         ; �Լ����� ����� ���� �������͸� ����
    ret             ; �Լ��� ȣ���� ���� �ڵ��� ��ġ�� ����

; ��Ʈ�� 1����Ʈ�� ��
;   PARAM: ��Ʈ ��ȣ, ������
kOutPortByte:
    push rdx        ; �Լ����� �ӽ÷� ����ϴ� �������͸� ���ÿ� ����
    push rax        ; �Լ��� ������ �κп��� ���ÿ� ���Ե� ���� ���� ����

    mov rdx, rdi    ; RDX �������Ϳ� �Ķ���� 1(��Ʈ ��ȣ)�� ����
    mov rax, rsi    ; RAX �������Ϳ� �Ķ���� 2(������)�� ����
    out dx, al      ; DX �������Ϳ� ����� ��Ʈ ��巹���� AL �������Ϳ� �����
                    ; �� ����Ʈ�� ��

    pop rax         ; �Լ����� ����� ���� �������͸� ����
    pop rdx
    ret             ; �Լ��� ȣ���� ���� �ڵ��� ��ġ�� ����


; GDTR �������Ϳ� GDT ���̺��� ����
;   PARAM: GDT ���̺��� ������ �����ϴ� �ڷᱸ���� ��巹��
kLoadGDTR:
    lgdt [ rdi ]    ; �Ķ���� 1(GDTR�� ��巹��)�� ���μ����� �ε��Ͽ�
                    ; GDT ���̺��� ����
    ret

; TR �������Ϳ� TSS ���׸�Ʈ ��ũ���� ����
;   PARAM: TSS ���׸�Ʈ ��ũ������ ������
kLoadTR:
    ltr di          ; �Ķ���� 1(TSS ���׸�Ʈ ��ũ������ ������)�� ���μ�����
                    ; �����Ͽ� TSS ���׸�Ʈ�� �ε�
    ret

; IDTR �������Ϳ� IDT ���̺��� ����
;   PARAM: IDT ���̺��� ������ �����ϴ� �ڷᱸ���� ��巹��
kLoadIDTR:
    lidt [ rdi ]    ; �Ķ���� 1(IDTR�� ��巹��)�� ���μ����� �ε��Ͽ�
                    ; IDT ���̺��� ����
    ret

; ���ͷ�Ʈ�� Ȱ��ȭ
;   PARAM: ����
kEnableInterrupt:
    sti             ; ���ͷ�Ʈ�� Ȱ��ȭ
    ret

; ���ͷ�Ʈ�� ��Ȱ��ȭ
;   PARAM: ����
kDisableInterrupt:
    cli             ; ���ͷ�Ʈ�� ��Ȱ��ȭ
    ret

; RFLAGS �������͸� �о �ǵ�����
;   PARAM: ����
kReadRFLAGS:
    pushfq                  ; RFLAGS �������͸� ���ÿ� ����
    pop rax                 ; ���ÿ� ����� RFLAGS �������͸� RAX �������Ϳ� �����Ͽ�
                            ; �Լ��� ��ȯ ������ ����
    ret

; Ÿ�� ������ ī���͸� �о ��ȯ
;	PARAM: ����
kReadTSC:
	push rdx				; RDX �������͸� ���ÿ� ����

	rdtsc					; Ÿ�� ������ ī���͸� �о RDX:RAX�� ����

	shl rdx, 32				; RDX �������Ϳ� �ִ� ���� 32��Ʈ TSC ���� RAX �������Ϳ� �ִ�
	or rax, rdx				; ���� 32��Ʈ TSC ���� OR�Ͽ� RAX �������Ϳ� 64��Ʈ TSC ���� ����

	pop rdx
	ret
