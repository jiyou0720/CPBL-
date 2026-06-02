import sys
import tty
import termios

# 터미널에서 엔터 없이 키 입력을 받기 위한 함수 (Linux 전용)
def get_key():
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setraw(sys.stdin.fileno())
        ch = sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    return ch

# 맵의 경계 설정 (가상 공간의 크기: 0 ~ 9)
MIN_X, MAX_X = 0, 9
MIN_Y, MAX_Y = 0, 9

# 초기 플레이어 위치 (X, Y)
p_x, p_y = 5, 5

print("=== 리눅스 이동 & 충돌 로직 테스트 ===")
print(f"시작 위치 -> X: {p_x}, Y: {p_y}")
print("[W: 위 / A: 왼쪽 / S: 아래 / D: 오른쪽 / Q: 종료]")
print("-" * 40)

while True:
    # 키 입력 받기
    key = get_key().lower()

    if key == 'q':
        print("\n프로그램을 종료합니다.")
        break

    # 1. 입력에 따른 '다음 가상 좌표' 미리 계산
    next_x, next_y = p_x, p_y

    if key == 'w':
        next_y -= 1  # 위로 이동 (Y축 감소)
    elif key == 's':
        next_y += 1  # 아래로 이동 (Y축 증가)
    elif key == 'a':
        next_x -= 1  # 왼쪽으로 이동 (X축 감소)
    elif key == 'd':
        next_x += 1  # 오른쪽으로 이동 (X축 증가)
    else:
        continue  # WASD 외의 키는 무시

    # 🛑 2. 충돌 처리 (경계면 벽 검사)
    # 계산된 다음 좌표가 설정한 맵의 범위를 벗어나는지 확인
    if next_x < MIN_X or next_x > MAX_X or next_y < MIN_Y or next_y > MAX_Y:
        print(f"❌ 충돌 발생! 더 이상 갈 수 없습니다. (시도한 좌표: X:{next_x}, Y:{next_y})")
        continue  # 실제 좌표를 갱신하지 않고 입력 대기로 돌아감

    # 3. 충돌하지 않았다면 실제 좌표 갱신
    p_x, p_y = next_x, next_y
    print(f"▶ 이동 완료 -> 현재 위치 - X: {p_x}, Y: {p_y}")
