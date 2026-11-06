<div align="center">
  <h1>📁 Thư mục Mã nguồn Cốt lõi (src)</h1>
  <p><b>Hệ thống Mã hóa và Giải mã Tệp tin Văn bản (Cipher Tool)</b></p>
  <p>Tài liệu kỹ thuật mô tả cấu trúc kiến trúc, chức năng chi tiết của từng mô-đun và luồng vận hành của ứng dụng mã hóa viết bằng ngôn ngữ C.</p>
</div>

<hr>

<h2>📊 Cấu trúc chi tiết các mô-đun mã nguồn</h2>

<table width="100%" border="1" cellpadding="8" cellspacing="0">
  <thead>
    <tr>
      <th align="left" width="25%">Tệp tin</th>
      <th align="left" width="75%">Mô tả chi tiết chức năng</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><b><code>main.c</code></b></td>
      <td>Điểm khởi chạy chính (Entry Point) của chương trình, chịu trách nhiệm phân tích tham số dòng lệnh (CLI arguments) và điều phối toàn bộ chu trình thực thi.</td>
    </tr>
    <tr>
      <td><b><code>cipher.h</code></b></td>
      <td>Tệp tiêu đề trung tâm định nghĩa các hằng số cấu hình, cấu trúc dữ liệu cơ sở và nguyên mẫu hàm cho các thuật toán mã hóa (Caesar, Vigenère, XOR).</td>
    </tr>
    <tr>
      <td><b><code>common.c</code> / <code>common.h</code></b></td>
      <td>Thư viện tiện ích dùng chung cung cấp các hàm hỗ trợ thao tác chuỗi ký tự, quản lý cấp phát bộ nhớ động và xử lý mã lỗi hệ thống.</td>
    </tr>
    <tr>
      <td><b><code>fileio.c</code> / <code>fileio.h</code></b></td>
      <td>Mô-đun quản lý nhập/xuất tệp tin (File I/O), thực hiện việc đọc dữ liệu nguồn và ghi kết quả xử lý an toàn vào tệp đích.</td>
    </tr>
    <tr>
      <td><b><code>fileinfo.c</code> / <code>fileinfo.h</code></b></td>
      <td>Thực hiện trích xuất, phân tích và kiểm tra các thông tin siêu dữ liệu (metadata) của tệp tin trước và sau quá trình mã hóa.</td>
    </tr>
    <tr>
      <td><b><code>header.c</code> / <code>header.h</code></b></td>
      <td>Xây dựng và xử lý cấu trúc phần tiêu đề tệp mã hóa (file header), lưu trữ thông tin về thuật toán, khóa và mã kiểm tra tính toàn vẹn CRC32.</td>
    </tr>
    <tr>
      <td><b><code>keyutil.c</code> / <code>keyutil.h</code></b></td>
      <td>Cung cấp các công cụ nhập liệu, xác thực định dạng, sinh và quản lý khóa mã hóa nhằm đảm bảo tính bảo mật cho thuật toán.</td>
    </tr>
    <tr>
      <td><b><code>adapter.c</code> / <code>adapter.h</code></b></td>
      <td>Lớp trung gian (Adapter Pattern) kết nối giữa giao diện điều khiển và tầng thuật toán mã hóa, chuẩn hóa phương thức gọi hàm.</td>
    </tr>
    <tr>
      <td><b><code>ui.c</code> / <code>ui.h</code></b></td>
      <td>Xây dựng giao diện dòng lệnh tương tác (TUI), hiển thị menu lựa chọn, hướng dẫn sử dụng và trạng thái tiến trình thực hiện.</td>
    </tr>
    <tr>
      <td><b><code>stub_identity.c</code></b></td>
      <td>Mô-đun định danh hỗ trợ quá trình liên kết tĩnh/động (linking) và kiểm thử đơn vị (unit testing) cho các thành phần độc lập.</td>
    </tr>
  </tbody>
</table>

<hr>

<h2>⚙️ Luồng vận hành hệ thống</h2>
<ol>
  <li><b>Khởi tạo và Phân tích tham số:</b> Chương trình khởi chạy từ <code>main.c</code>, tiếp nhận yêu cầu từ người dùng thông qua tham số dòng lệnh hoặc giao diện tương tác <code>ui.c</code>.</li>
  <li><b>Thiết lập Khóa và Tiêu đề:</b> Mô-đun <code>keyutil.c</code> xác thực và chuẩn bị khóa mã hóa, trong khi <code>header.c</code> thiết lập cấu trúc tiêu đề tệp đích.</li>
  <li><b>Xử lý Dữ liệu:</b> <code>fileio.c</code> đọc dữ liệu từ tệp nguồn theo từng khối, sau đó chuyển giao cho <code>adapter.c</code> và các thuật toán mã hóa tương ứng để biến đổi nội dung.</li>
  <li><b>Kiểm tra toàn vẹn và Lưu trữ:</b> Hệ thống tính toán mã kiểm tra, đóng gói thông tin vào tiêu đề tệp và ghi toàn bộ dữ liệu hoàn chỉnh xuống tệp đích một cách an toàn.</li>
</ol>

<br><br>

<div align="right" style="float: right; text-align: center; width: 320px;">
  <p><i>TP. Hồ Chí Minh, ngày 08 tháng 11 năm 2026</i></p>
  <p><b>Kẻ gánh team</b></p>
  
 <h2><b>Chatgpt and Le Ngoc Vuong</b></h2>
</div>
<div style="clear: both;"></div>
