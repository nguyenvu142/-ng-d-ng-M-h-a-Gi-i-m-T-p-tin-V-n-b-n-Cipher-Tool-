<div align="center">
  <h1>📁 Thư mục Mã nguồn Cốt lõi (src)</h1>
  <p><b>Ứng dụng Mã hóa & Giải mã Tệp tin Văn bản (Cipher Tool)</b></p>
  <p>Thư mục này chứa toàn bộ mã nguồn C cốt lõi thực thi các thuật toán mã hóa, xử lý tệp tin, quản lý khóa và điều phối hệ thống.</p>
</div>

<hr>

<h2>📊 Cấu trúc các tệp tin mã nguồn</h2>

<table width="100%" border="1" cellpadding="8" cellspacing="0">
  <thead>
    <tr>
      <th align="left" width="25%">Tệp tin</th>
      <th align="left" width="75%">Vai trò và Chức năng chính</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><b><code>main.c</code></b></td>
      <td>Điểm khởi chạy chính (Entry point) của chương trình, phân tích tham số dòng lệnh (CLI) và điều hướng các chức năng mã hóa/giải mã.</td>
    </tr>
    <tr>
      <td><b><code>cipher.h</code></b></td>
      <td>Tệp tiêu đề tổng hợp cấu trúc dữ liệu, hằng số chung và nguyên mẫu hàm cho hệ thống mã hóa.</td>
    </tr>
    <tr>
      <td><b><code>common.c</code> / <code>common.h</code></b></td>
      <td>Các hàm tiện ích dùng chung (xử lý chuỗi, cấp phát bộ nhớ, quản lý lỗi) được sử dụng xuyên suốt dự án.</td>
    </tr>
    <tr>
      <td><b><code>fileio.c</code> / <code>fileio.h</code></b></td>
      <td>Quản lý các thao tác nhập/xuất tệp tin (File I/O), đọc dữ liệu nguồn và ghi kết quả mã hóa/giải mã an toàn.</td>
    </tr>
    <tr>
      <td><b><code>fileinfo.c</code> / <code>fileinfo.h</code></b></td>
      <td>Trích xuất, kiểm tra và quản lý siêu dữ liệu (metadata) của tệp tin trước và sau khi xử lý.</td>
    </tr>
    <tr>
      <td><b><code>header.c</code> / <code>header.h</code></b></td>
      <td>Định dạng và xử lý cấu trúc phần tiêu đề (header) đính kèm trong tệp mã hóa (lưu trữ thông tin thuật toán và cờ toàn vẹn).</td>
    </tr>
    <tr>
      <td><b><code>keyutil.c</code> / <code>keyutil.h</code></b></td>
      <td>Cung cấp công cụ nhập, xác thực, sinh và quản lý khóa mã hóa (key management) cho các thuật toán.</td>
    </tr>
    <tr>
      <td><b><code>adapter.c</code> / <code>adapter.h</code></b></td>
      <td>Lớp trung gian (adapter) kết nối giữa giao diện điều khiển và các thuật toán mã hóa, chuẩn hóa giao diện gọi hàm.</td>
    </tr>
    <tr>
      <td><b><code>ui.c</code> / <code>ui.h</code></b></td>
      <td>Xây dựng giao diện dòng lệnh (Text-based User Interface), hiển thị menu, hướng dẫn sử dụng và trạng thái tiến trình.</td>
    </tr>
    <tr>
      <td><b><code>stub_identity.c</code></b></td>
      <td>Tệp mô-đun định danh/stub hỗ trợ quá trình liên kết (linking) và kiểm thử các thành phần độc lập.</td>
    </tr>
  </tbody>
</table>

<hr>

<h2>⚙️ Luồng hoạt động cơ bản</h2>
<ol>
  <li><b>Khởi động (<code>main.c</code>):</b> Nhận lệnh từ người dùng qua dòng lệnh hoặc thông qua giao diện tương tác (<code>ui.c</code>).</li>
  <li><b>Xử lý khóa & Tham số:</b> Sử dụng <code>keyutil.c</code> để chuẩn bị khóa và <code>header.c</code> để thiết lập cấu trúc tệp đích.</li>
  <li><b>Đọc & Mã hóa:</b> <code>fileio.c</code> đọc dữ liệu tệp nguồn, sau đó <code>adapter.c</code> chuyển dữ liệu qua các thuật toán mã hóa tương ứng.</li>
  <li><b>Ghi tệp:</b> Kết quả sau khi xử lý được ghi lại hoàn chỉnh vào tệp đích thông qua hệ thống quản lý tệp tin an toàn.</li>
</ol>

<br><br>

<div align="right" style="float: right; text-align: center; width: 300px;">
  <p><i>TP. Hồ Chí Minh, ngày 08 tháng 11 năm 2026</i></p>
  <p><b>Kẻ gánh team</b></p>
  <br>
  <p><b>Chatgpt and Le Ngoc Vuong</b></p>
</div>
<div style="clear: both;"></div>
