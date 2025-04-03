BÁO CÁO BÀI TẬP LỚP MÔN LẬP TRÌNH NÂNG CAO

Sinh viên: Trần Gia Hoàng Nam	

Sinh ngày: 19/02/2006 

Ngành học: Công nghệ thông tin

Lớp: K69I-IT3

Tên game : ……………SLIME ADVENTURE……………………………………………

Lối chơi, logic của game:
Dựa trên tựa game Bomb IT hay Bomb Online (Crazy Arcade): Điều khiển và đặt bomb để đánh bại càng nhiều quái vật càng tốt.

Đồ họa, âm thanh:

Về phần Đồ hoạ âm thanh được chia thành 4 file chính (có đính kèm trong file):

+) font: Chủ yếu dùng 2 font Karma Future, VCR_OSD_MONO_1.001 để ghi lại điểm số cũng như hiển thị các thông số của người chơi

+) images: - Game Over: gồm 3 ảnh khác nhau tùy vào điểm số người chơi ( điểm < 10: Try better,  điểm:  < 50 That good, điểm: >= 50 Amazing)

Player, Enemies: gồm các frame đứng đợi (Idle), di chuyển (Walk), tấn công (Attack), chết (Death), hiệu ứng (Effect), có tổng cộng 4 loại khác nhau

Menu, Pause, Score, Tutorial, …

Gồm một số Frame của grass, rock, shurub để linh hoạt trong việc thay đổi khí hậu

+) maps: Hiện tại gồm 3 level (có thể bổ sung thêm tùy thích)

+) sounds: Gồm các âm thanh khi ấn chuột, đặt bom, nổ bom, mất máu, chết, …

Cấu trúc của project game:

+) assets: lưu âm thanh, hình ảnh, level màn chơi của game

+) game:

- main: khởi tạo game 
- game: quản lí các logic, trình tự của game
- object:
1. bomb: vị trí + thời gian nổ bom
2. explosion: kiểm soát thời gian và frame của vụ nổ
- map : xử lí các logic có liên quan đến bản đồ: tải hình ảnh, sửa bản đồ, tạo vật phẩm ngẫu nhiên
- player: người chơi
- render: Quản lí, lưu trữ và tải hình ảnh của toàn bộ game
- enmies: AI của quái vật
- inputHandler: Quản lí thông tin input khi game chạy
- Info: Lưu trữ các biến số ban đầu của Game, một số phép toán logic

Các chức năng đã cài được cho game:

+) Các frame hành động được tinh chỉnh phù hợp để chuyển động mượt mà đặc biệt là các bước di chuyển.  

+) Quái vật có thể tránh được vật cản và truy đuổi người chơi một cách tự động có thể tấn công khi vào tầm và phá hủy những vật thể cản đường.

+) Quái vật có hiệu ứng, tốc độ ra đòn, tốc độ di chuyển, tốc độ xử lí khác nhau (có thể tự điều chỉnh)

+) Quái vật có cách tấn công khác nhau và được tạo một cách có chủ đích tùy vào điểm số hiện tại của người chơi để làm tăng độ khó một cách ổn định.

+) Đã có các vật phẩm như: tăng kích thước bom, tăng số lượng bom, hồi máu, tăng khiên để tăng sức mạnh cho người chơi trong trận đấu.

+) Vật phẩm có xác xuất ngẫu nhiên rơi ra khi phá hủy vật thể hoặc tiêu diệt địch.

+) Có âm thanh đầy đủ cho background, đặt bom, nổ bom, ăn vật phẩm, mất máu, chết.

+) Thông số của người chơi có thể tùy chỉnh trực tiếp trong MainMenu và được lưu lại xuyên suốt quá trình chơi.

+) Người chơi có thể điều chỉnh giữa 3 khí hậu: Xuân, Thu, Đông với 3 nền ảnh, môi trường khác nhau.

+) Các bản đồ được tải ngẫu nhiên từ bể bản đồ có sẵn (hiện tại có 3) có thể cho thêm một cách tùy thích.

+) Hướng dẫn được popup mỗi khi gặp chủng loại địch mới.

+) Các nút bấm đều ổn định ít tình trạng giật lag.
