# 不思議のダンジョンランダム生成
2015年、大学１年生の終盤に習作兼作りたいゲームのパーツとして開発したもの。<br>
開発期間は２日程度。<br>
生成過程をいい感じに分割して、関数の呼び出し方次第で、<br>
ダンジョンに特色を出せるように工夫してある。<br>
<br>
まだ良くわかってない時期で、ヘッダじゃなくてCファイルに頑張って関数の解説書いてたりする。<br>
make_dungeonXみたいに何個も似たソースコードおいてあったけどgit使おうとは当時は思いつきもしなかった様子。<br>
そのせいで今見るとどれが最新かもわかりにくい。<br>
途中でライブラリ化しようと思いたち、VS環境での開発に切り替えた様子（CPP版）。<br>
そっちのヘッダを見た感じ、不自然なことに公開する関数側からは、ダンジョンの生成過程の関数が見えてないため、ライブラリを改造しながらの個人開発しか想定出来ていなかった様子。<br>
あと、ライブラリ化はおいおいでいいやと、途中でやめたっぽい。<br>
ヘッダもだけど、処理自体はいいとしても、今見ると再利用するにも整備が必要そう。<br>
以下、実行例。<br>
<img src="https://github.com/asamichi/make_dungeon/blob/master/screen/1.jpg"><br>
<img src="https://github.com/asamichi/make_dungeon/blob/master/screen/2.jpg"><br>
<img src="https://github.com/asamichi/make_dungeon/blob/master/screen/3.jpg"><br>
<img src="https://github.com/asamichi/make_dungeon/blob/master/screen/4.jpg"><br>
<img src="https://github.com/asamichi/make_dungeon/blob/master/screen/5.jpg"><br>