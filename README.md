# Flame Dragon X (FD2)

Cocos2d-x 3.12 기반 시뮬레이션 RPG 리메이크. 원본 PC-98 시절 火焰之纹章 II를 모티브로 함.

본 README는 **Windows에서 빌드 & 실행**을 다룹니다.

## 요구사항

- **Windows 10 / 11**
- **Visual Studio 2022** (Community 이상)
  - 워크로드: **C++를 사용한 데스크톱 개발**
  - 개별 구성 요소: **MSVC v143**, **Windows 11 SDK** (10.0.26100 이상 권장)
- **Python 3.x** (cocos2d-x prebuilt 의존성 다운로드용)

## 빌드 (로컬)

### 1. cocos2d-x 외부 의존성 다운로드

처음 한 번만 실행하면 `cocos2d/external/`에 prebuilt 라이브러리(curl, freetype, png 등)가 받아집니다.

```cmd
cd cocos2d
python download-deps.py -r yes
```

### 2. Visual Studio로 빌드

옵션 A — IDE에서:
1. `proj.win32/FlameDragonX.sln` 열기
2. 구성: `Debug | Win32` (또는 `Release | Win32`)
3. `Build → Build Solution` (Ctrl+Shift+B)

옵션 B — 명령줄(MSBuild):

```powershell
$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
& $msbuild proj.win32\FlameDragonX.sln `
    /t:FlameDragonX `
    /p:Configuration=Debug `
    /p:Platform=Win32 `
    /p:PlatformToolset=v143 `
    /m
```

성공하면 `proj.win32/Debug.win32/FlameDragonX.exe`가 생성되고 의존 DLL과 `Resources/`가 같은 폴더로 자동 복사됩니다.

## 실행

```
proj.win32\Debug.win32\FlameDragonX.exe
```

또는 `run_with_log.bat`을 사용하면 stdout/stderr이 같은 폴더의 `stdout.txt` / `stderr.txt`로 저장됩니다 (디버깅용).

창 크기는 1440×960 (디자인 480×320의 정확히 3배, 픽셀 퍼펙트).

## 조작법

### Title 화면
- **마우스**: 메뉴 버튼 클릭
- **← → ↑ ↓**: Start / Load / Continue 사이 이동 (선택된 버튼 1.7배 확대)
- **Enter / Space**: 선택 활성화
- **S** 또는 우상단 "Settings" 클릭: 설정 화면 진입
- **ESC**: 종료

### Battle 화면
- **마우스 클릭**: 격자 선택
- **← → ↑ ↓**: 커서 한 칸씩 이동 (필드 범위 [1, fieldWidth] × [1, fieldHeight])
- **Enter / Space**: 현재 커서 위치 클릭 = 메뉴/유닛 선택
- **ESC**: 떠 있는 메뉴 닫기

### Village 화면
- **← ↑** / **→ ↓**: 6개 위치 사이 순환
- **Enter / Space**: 현재 위치 진입 (0번 = 종료 확인, 1~5번 = 상점)
- **ESC**: 종료 확인

### 대화창 (스토리 진행 중)
- **마우스 클릭** / **Enter** / **Space**: 다음 메시지로 진행

### Setting 화면
- **↑ ↓**: 항목 이동
- **← →**: 현재 항목 값 변경
- **Enter / Space**: 토글/순환 (Back 항목에서는 복귀)
- **ESC**: Title로 복귀

## 설정 (Setting 메뉴)

| 항목 | 옵션 | 기본값 | 설명 |
|---|---|---|---|
| **Story Events** | ON / OFF | OFF | 챕터 1 라운드별 컷씬·대화·적 등장 이벤트 활성화 |
| **Dialog Font Size** | 12 / 14 / 18 / 24 | 14 | 대화창 한글 폰트 크기 |

설정은 `UserDefault`에 저장되어 앱 재시작 후에도 유지됩니다.

## CI / 자동 Release

`.github/workflows/release.yml`이 다음을 자동화합니다.

| 트리거 | 동작 |
|---|---|
| `git push origin v*` (예: `v0.1.0`) | windows-latest 러너에서 Release 빌드 → GitHub Release 자동 생성 + `FlameDragonX-v0.1.0.zip` 첨부 |
| GitHub Actions UI에서 **Run workflow** | Release 빌드 후 Workflow Artifact으로 30일 보관 |

릴리즈 만들기:
```bash
git tag v0.1.0
git push origin v0.1.0
```

zip 안에는 `FlameDragonX.exe` + 의존 DLL + `Resources/`가 모두 들어 있어 그대로 실행 가능합니다.

## 알려진 사항

- v143 컴파일러의 PDB 락 충돌 회피를 위해 `DebugInformationFormat`을 `OldStyle(/Z7)`로 사용합니다.
- 챕터 1 라운드 1 대화는 한국어로 번역되어 있습니다 (`Resources/Strings/zh-cn/Maps/Chapter-01.strings` 내). 다른 챕터/UI 텍스트는 아직 중국어입니다.
- 한글 표시를 위해 `Resources/fonts/malgun.ttf`(맑은 고딕)이 추가되어 있습니다. 라이선스를 고려해 배포가 필요한 경우 NanumGothic 등 OFL 폰트로 교체하세요.
- 원본 게임은 마우스/터치 전용으로 작성되어 있어 키보드 지원은 본 포크에서 추가한 것입니다 (Title / Battle / Village / Setting / 대화창).

## 디렉토리 구조

```
.
├── Classes/                 게임 코드 (C++)
│   ├── Scenes/              Title, Setting, Loading, Shopping, Village...
│   ├── Battle/              BattleScene, BattleField, AI, Events
│   ├── Activities/          애니메이션·동작 큐
│   ├── Data/                창조물·아이템·마법 정의
│   └── ...
├── Resources/               리소스 (이미지, 폰트, strings, .dat)
├── cocos2d/                 cocos2d-x 3.12 엔진
└── proj.win32/              Visual Studio 솔루션 (이 README가 다루는 빌드)
```
