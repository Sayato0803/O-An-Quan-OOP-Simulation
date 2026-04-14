\# TÀI LIỆU ĐẶC TẢ LUẬT CHƠI Ô ĂN QUAN (DÀNH CHO CORE ENGINE)



Tài liệu này là cơ sở đối chiếu (Source of Truth) để Thành viên 1 \& 2 code các class `Board`, `Pit`, và `Player`. Bất kỳ logic code nào chạy sai với tài liệu này đều bị coi là BUG.



\## 1. Cấu trúc Bàn cờ (Board Configuration)

\* Bàn cờ là một mảng vòng tròn (Circular Array) gồm \*\*12 ô\*\* (Index từ 0 đến 11).

\* \*\*Ô Quan (Mandarin Pits):\*\* Nằm ở 2 đầu, index là `0` và `6`. 

\* \*\*Ô Dân (Citizen Pits):\*\* \* Player 1 quản lý các ô index `1, 2, 3, 4, 5`.

&#x20; \* Player 2 quản lý các ô index `7, 8, 9, 10, 11`.

\* \*\*Trạng thái khởi tạo (Initial State):\*\* \* Mỗi ô Quan có 1 viên sỏi lớn (Quy ước giá trị = 10 điểm).

&#x20; \* Mỗi ô Dân có 5 viên sỏi nhỏ (Quy ước giá trị = 1 điểm).

&#x20; \* Tổng giá trị trên bàn cờ khi bắt đầu: (2 \* 10) + (10 \* 5) = 70 điểm.



\## 2. Luật Bốc \& Rải quân (Sowing Logic)

\* Đến lượt Player nào, người đó chỉ được quyền chọn 1 ô Dân \*\*khác 0\*\* thuộc phần sân của mình để bốc.

\* \*\*Không bao giờ\*\* được phép chọn bốc từ ô Quan.

\* Lấy toàn bộ số quân trong ô đã chọn, rải lần lượt mỗi ô 1 viên theo một chiều tùy chọn (cùng chiều kim đồng hồ hoặc ngược chiều).



\## 3. Luật Dừng \& Chững (Turn Ending)

Sau khi thả viên sỏi \*\*cuối cùng\*\* xuống một ô (gọi là ô A), ta xét \*\*ô tiếp theo\*\* (gọi là ô B):

\* \*\*Trường hợp 1 (Rải tiếp):\*\* B là ô Dân và có quân -> Bốc toàn bộ quân ở B và rải tiếp.

\* \*\*Trường hợp 2 (Mất lượt):\*\* B là ô trống, ô tiếp sau B (ô C) CŨNG LÀ ô trống -> Nhường lượt cho đối phương.

\* \*\*Trường hợp 3 (Chững - Mất lượt):\*\* B là ô Quan (bất kể có quân hay trống) -> Chững lại, nhường lượt cho đối phương. \*(⚠️ Lưu ý cho Coder: Chỗ này hay nhầm, tuyệt đối không được bốc quân từ ô Quan để rải đi).\*



\## 4. Luật Ăn quân (Capture Logic - Rất quan trọng)

\* \*\*Điều kiện ăn:\*\* B là ô trống, ô tiếp sau B (ô C) CÓ QUÂN.

\* \*\*Hành động:\*\* Player cộng toàn bộ điểm của ô C vào quỹ điểm của mình, set giá trị ô C về 0.

\* \*\*Luật Ăn Liên Hoàn (Combo):\*\* Sau khi ăn ô C, xét tiếp ô D và ô E. Nếu ô D trống và ô E có quân -> Tiếp tục ăn ô E. Quá trình lặp lại cho đến khi gặp 2 ô trống liên tiếp hoặc gặp ô Quan (tùy luật địa phương, nhóm mình thống nhất \*\*ĐƯỢC\*\* ăn liên hoàn qua Quan nếu đúng nhịp Trống - Có).



\## 5. Luật Cấp Vốn / Vãi Quân (Starvation Rule)

Đến lượt của Player, nếu kiểm tra thấy \*\*cả 5 ô Dân của mình đều bằng 0\*\*:

\* Bắt buộc phải trừ 5 điểm trong quỹ điểm đang có.

\* Rải đều 5 viên đó vào 5 ô Dân của mình (mỗi ô 1 viên).

\* Nếu quỹ điểm không đủ 5 (ví dụ quỹ = 0): "Mượn" của đối phương (ghi nợ -5) để rải rồi chơi tiếp. Cuối game trừ lại.



\## 6. Điều kiện Kết thúc (End Game Trigger)

\* Trò chơi kết thúc ngay lập tức khi \*\*CẢ 2 Ô QUAN ĐỀU TRỐNG\*\* (Giá trị = 0).

\* Ô Quan trống 1 bên thì vẫn chơi bình thường.

\* \*\*Thanh lý:\*\* Khi game over, số dân còn tồn đọng bên sân nhà ai thì tự động cộng vào điểm của người đó.

\* \*\*Phân định:\*\* So sánh tổng điểm.

