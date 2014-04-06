#!/bin/bash

cd wxWidgets_vdm_patch && ./X64/Release/wxWidgets_vdm_patch.exe -p --no-wait && cd .. && rm -f build/msw/*.bcc build/msw/*.dsp build/msw/*.dsw build/msw/*.gcc build/msw/wx_vc10* build/msw/wx_vc7* build/msw/wx_vc8* build/msw/wx_vc9* build/msw/wx_vc10* build/msw/wx_vc11* build/msw/wx_vc12* build/msw/*.vc build/msw/*.wat build/msw/*.props

#eof
