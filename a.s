section .text
global main
extern printNumber
extern scanNumber
main:
  push rbp
  mov rbp, rsp
  sub rsp, 208
  mov rax, rbp
  sub rax, 8
  push rax
  pop rdi
  call scanNumber
  mov rax, rbp
  sub rax, 16
  push rax
  pop rdi
  call scanNumber
  mov rax, rbp
  sub rax, 8
  push rax
  pop rax
  mov rax, [rax]
  push rax
  mov rax, rbp
  sub rax, 16
  push rax
  pop rax
  mov rax, [rax]
  push rax
  pop rdi
  pop rax
  imul rax, rdi
  push rax
  pop rdi
  call printNumber
  push 1
  pop rax
  leave
  ret
