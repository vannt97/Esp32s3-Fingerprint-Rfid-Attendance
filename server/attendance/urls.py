from django.urls import path

from .views import AttendanceCreateView, EmployeeListView

urlpatterns = [
    path("employees/", EmployeeListView.as_view(), name="employee-list"),
    path("attendance/", AttendanceCreateView.as_view(), name="attendance-create"),
]
