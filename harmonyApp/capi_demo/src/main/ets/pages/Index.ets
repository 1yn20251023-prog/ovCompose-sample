import { hilog } from '@kit.PerformanceAnalysisKit';
import nativeNode from 'libnativeNode.so';

const DOMAIN = 0x0000;

@Entry
@Component
struct Index {
  @State message: string = 'Hello World';

  build() {
    Row() {
      Column() {
        XComponent({
          id: 'xcomponentId',
          type: 'node',
          libraryname: 'nativeNode'
        }).onAppear(() => {
          nativeNode.createNativeNode('xcomponentId', this.getUIContext());
        })
          .width('100%')
          .height('100%')
          .id('xcomponentId')
          .backgroundColor(Color.White)
      }
      .width('100%')
    }
    .height('100%')
  }
}
