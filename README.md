### 임베디드 시계, 날씨 프로그램(개인) - Embedded

1. 개요

   - ARM 프로세서 기반에 임베디드 리눅스에서 7-segment에 현재 시각 또는 타이머를 표시하고, 현재 날씨 정보를 받아와 8x8 도트 매트릭스 상에 이모티콘으로 재생하는 프로그램을 개발했습니다. 

     ![demo](https://user-images.githubusercontent.com/23726218/118643217-394c6a80-b817-11eb-9ae3-a52bfb01e81b.png)

2. 특징

   - 날씨 정보 받아오기

     임베디드 리눅스에 있던 WGET을 통해 기상청에서 제공하는 날씨 XML을 다운로드하고 파싱을 통해 날씨 정보를 추출해 냈습니다. 장치의 입력을 계속해서 감지하고 도트매트릭스 또는 7-segment로 계속 출력해야 했기 때문에 날씨정보 다운로드 및 파싱을 자식 프로세스를 통해서 동시에 진행할 수 있도록 했습니다.

   - 도트 매트릭스

     어떤 LED가 켜질지는 장치 쓰기 하는 바이너리 값에 달려 있었기 때문에 이모티콘의 바이너리를 직접 계산해서 상수로 정의할 수도 있었지만 수작업으로 진행해야 하고 수정에 취약하다는 점에서 너무 비효율적이라는 생각이 들었습니다. 

     따라서 메인 로직과 그래픽 관련 로직을 분리해서 점들의 집합, 직선, 원을 표현하는 구조체를 정의하고 이러한 구조체들을 원형 리스트에 담아서 보내기만 하면 각 도형의 좌표들을 계산하고 바이너리 값을 계산해서 8x8 매트릭스 디바이스 쓰기 작업을 통해서 실제로 불이 들어오도록 만들었습니다.

     ```c
     // emoticon.c
     void playEmoticon(int Weather)
     {
         int EmoticonOffset = (Weather - 1) * 2;
         Drawable dFrame_1 = {.type = TYPE_Points, .Points = &EMOTICONS[EmoticonOffset]};
         Drawable dFrame_2 = {.type = TYPE_Points, .Points = &EMOTICONS[EmoticonOffset + 1]};
     
         RenderQueue Frame1, Frame2;
     
         Initialize(&Frame1);
         Initialize(&Frame2);
     
         InsertFront(&Frame1, &dFrame_1);
         InsertFront(&Frame2, &dFrame_2);
     
         if (AnimationFrame % 2 == 0){
             DOT_Draw_Canvas(&Frame1);
         }else{
             DOT_Draw_Canvas(&Frame2);
         }
         AnimationFrame++;
     }
     ```

     > 메인 로직에서 각각의 LED가 어떻게 켜지는지 몰라도 되도록 추상화했습니다.

     ```c
     //Dot_graphic_library.c
     void DOT_Draw_Canvas(RenderQueue *renderQueue){
         char MappedPoints[DOT_SIZE][DOT_SIZE] = INIT_POINTS;
         RowBits Frames[DOT_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
     
         while (renderQueue->cur != NULL){
             Drawable D = renderQueue->cur->value;
     
             switch (D.type){
             case TYPE_Points:
                 MappingPointsToDotMatrix(MappedPoints, D.Points);
                 break;
     
             case TYPE_Line:
                 MappingLineToDotMatrix(MappedPoints, D.line);
                 break;
     
             case TYPE_Circle:
                 MappingCircleToDotMatrix(MappedPoints, D.circle);    
                 break;
     
             default:
                 printf("Drawble Type invalid! : %d\n", D.type);
                 break;
             }
             
             MoveNext(renderQueue);
         }
         //그리기
         Generate_Hex_Code(Frames, MappedPoints);
         RenderingFrame(Frames);
         //커서 맨앞자리로 다시 옮겨놓기
         renderQueue->cur = renderQueue->head;
     }
     ```

     > 그려질 대상이 담겨있는 원형큐를 순회하며 LED에 쓰일 바이너리 값을 계산할 수 있도록 했습니다.

   - 입력과 출력 동시에 받기

     입출력 라인에 제한이 있었기 때문에 스위치와 도트 매트릭스, 7-segment를 동시에 읽거나 쓸 수 없었습니다. 따라서 스위치를 누를 때 즉각적으로 반응하면서 화면은 계속해서 나타나도록 하기 위해서 메인 프로세스는 스위치 입력 감지와 7-segment, 도트 매트릭스 쓰기로 이루어진 한 사이클을 100ms마다 반복하도록 구현했습니다.
