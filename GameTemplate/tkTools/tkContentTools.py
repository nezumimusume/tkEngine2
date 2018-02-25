# -*- coding: utf-8 -*-
#tkContentTools
#AssetsフォルダのファイルをResourceフォルダにコピーするスクリプト。

import shutil
import sys
import os

args = sys.argv
assetPath = args[1]
outputPath = args[2] + assetPath
#コピー先のパスに変換する。
resourcePath = assetPath.replace("Assets", "Resource");
#ディレクトリ名のみ取得する。
dict, file = os.path.split(resourcePath);

if os.path.exists(dict) == False :
	os.makedirs(dict)
#コピー。
shutil.copy2(assetPath, resourcePath);

#出力フォルダにもコピー。

dict, file = os.path.split(outputPath);
if os.path.exists(dict) == False:
	os.makedirs(dict)
#コピー。
shutil.copy2(assetPath, outputPath);
