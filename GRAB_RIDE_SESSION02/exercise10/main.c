#include <stdio.h>

int main() {
    double distance_km;
    int is_surge;
    double base_fare = 0.0;
    double total_fare = 0.0;

    printf("Nhap vao quang đường di chuyển (km): ");
    if (scanf("%lf", &distance_km) != 1) {
        printf("Loi: Du lieu nhap vao khong hop le!\n");
        return 1;
    }

    printf("Nhap trang thai phu phi (0: Binh thuong, 1: Gio cao diem): ");
    if (scanf("%d", &is_surge) != 1) {
        printf("Loi: Du lieu nhap vao khong hop le!\n");
        return 1;
    }

    // Guard Clause 1: Kiem tra du lieu quang đường
    if (distance_km <= 0.0) {
        printf("Loi: Quang đường khong duoc nho hon hoac bang 0 km!\n");
        return 1;
    }

    // Guard Clause 2: Kiem tra trang thai phu phi
    if (is_surge != 0 && is_surge != 1) {
        printf(
            "Loi: Trang thai phu phi chi duoc phep la 0 hoac 1!\n"
        );
        return 1;
    }

    // Buoc tinh toan cước cơ bản theo bước (Step-by-Step)
    if (distance_km <= 2.0) {
        base_fare = 12000.0;
    } else {
        base_fare = 12000.0 + (distance_km - 2.0) * 4500.0;
    }

    total_fare = base_fare;

    // Ap dung phu phi neu co
    if (is_surge == 1) {
        total_fare = total_fare * 1.2;
    }

    // Hien thi ket qua cuoi cung
    printf("----------------------------------------\n");
    printf("THONG TIN CHI TIET CHUYEN DI GRABRIDE\n");
    printf("Quang đường: %.2f km\n", distance_km);
    printf("Trang thai cao diem: %s\n", is_surge == 1 ? "Co (Tang 20%)" : "Khong");
    printf("Tong chi phi cuoi cung: %.2f VNĐ\n", total_fare);
    printf("----------------------------------------\n");

    return 0;
}
