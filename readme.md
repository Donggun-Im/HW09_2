## 숫자 야구 게임

---
### 목차
---

- 기본 게임 로직
- 네트워크 통신
- 턴 기능 추가

---
### 개요 
---

프로젝트 이름: HW09
개발 엔진 및 언어 : Unreal Engine & C++
과제 제출자 : 임동건

<img width="3840" height="1080" alt="hw09readmeimage" src="https://github.com/user-attachments/assets/8fe51f6e-e2dc-4ec4-9534-90c7c411a2d7" />


---
### 게임 설명 
---

플레이어들은 랜덤하게 생성된 1~9의 다른 숫자 3개로 이루어진 세자리 숫자를 찾아야 한다.
플레이어 1부터 끝 번호까지 턴이 진행되며 각 턴은 최대 30초까지 지속되지만 플레이어가 자신의 답을 입력하면 즉시 턴이 종료된다.
숫자의 값과 자리가 맞으면 1S 값만 맞으면 1B을 얻는다.

예시) 정답 = 123, 플레이어 1번의 답 = 132,  출력문 = PLAYER1(1/3) : 1S2B

플레이어 1번부터 끝번까지 모두 정답을 맞추지 못한채로 턴을 다 쓸 경우 Draw.. 가 출력되며 다시 처음부터 게임 진행
정답을 맞출경우 PLAYER번호 has won the game이 출력되며 게임 종료.

----
### 플레이 방식
---

콘솔창 상단에 나오는 턴과 자신의 턴이 일치할 때 3개의 숫자로 끝나는 문자열을 입력하면 그 3개의 숫자로 정답 확인
자신의 턴이 아니거나 끝이 3자리 숫자가 아닌 문자열을 입력하면 기본적인 통신 기능만 지원.

---
### 제작 설명
---

게임 모드에선 게임 진행 과정에서 필요한 판정과 게임 진행 로직을 구현

``` cpp
public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void BeginPlay() override;
	void PrintChatMessageString(AHW09PlayerController* InPlayerController, FString InChatMessageString);
	UPROPERTY()
	TArray<TObjectPtr<AHW09PlayerController>> AllPlayerControllers;
	UFUNCTION()
	bool IsDraw();
	bool bIsDraw;
protected:
	FString MakeRandomNumbers();
	FString JudgeNumbers(FString InGuessNumber);
	void JudgeGame(AHW09PlayerController* InPlayerController,int32 StrikeCount);
	bool IsGuessNumber(FString InString, AHW09PlayerController* InPlayerController);
	void IncreaseGuessCount(AController* InController);
	void ResetGame();
```

게임 스테이트에선 게임 진행과 관련된 데이터 관리. (주로 턴과 플레이어를 전체 관리하기 위한 데이터 및 함수)

``` cpp
	UPROPERTY()
	int32 WhoseTurn;
	FString GetSecretNumber()
	{
		return SecretNumber;
	};
	int32 GetWhoseTurn()
	{
		return WhoseTurn;
	};
	void SetSecretNumber(FString InSecretNumber)
	{
		SecretNumber = InSecretNumber;
	};
	void SetWhoseTurn(int32 InWhoseTurn)
	{
		WhoseTurn = InWhoseTurn;
	}
	UFUNCTION()
	int32 GetPlayerTurn() { return WhoseTurn; };
	UFUNCTION()
	void SetPlayerTurn();
	UFUNCTION()
	void ShowPlayerTurn();
	UFUNCTION()
	void MulticastRPCBroadCastingLoginMessage(FString InMessageString = FString(TEXT("XXXXX")));
	UPROPERTY()
	int32 PlayerCount =1;
	UPROPERTY()
	float TimePerTurn = 30.0f;
	UPROPERTY()
	bool bIsGameStart = false;
private:
	FString SecretNumber;
	
	FTimerHandle TurnTimerHandle;

```

PlayerController에서는 문자열 입력 및 UI출력 관리

``` cpp
public:
	virtual void BeginPlay() override;
	AHW09PlayerController();
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintMessageString(const FString& InChatMessageString);
	UFUNCTION()
	void SetChatMessageString(const FString& InChatMessageString);
	void PrintMessageString(const FString& InChatMessageString);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHW09InputWidget> ChatUserWidgetClass;
	UPROPERTY()
	TObjectPtr<UHW09InputWidget> ChatUserWidgetInstance;

	FString ChatMessageString;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHW09UserWidget> NotificationTextWidgetClass;
	UPROPERTY()
	TObjectPtr<UHW09UserWidget> NotificationTextWidgetInstance;
public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
```

플레이어 스테이트 에서는 각 플레이어의 상태 및 정보를 관리한다.

``` cpp
	AHW09PlayerState();
	virtual void BeginPlay() override;
	FString GetPlayerInfoString();
	UPROPERTY(Replicated)
	FString PlayerName;
	UPROPERTY(Replicated)
	int32 PlayerNum =0;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	int32 MaxGuessCount =3 ;
	UPROPERTY(Replicated)
	int32 CurrentGuessCount =0;
```








