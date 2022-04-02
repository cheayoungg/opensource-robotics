package com.example.androidproject1;

import android.app.Activity;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Locale;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.ListAdapter;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class MonthActivity extends AppCompatActivity {
    TextView tvDate;
    Calendar Cal;
    GridView gridView;
    ArrayList<String> dayList;
    GridAdapter gridAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // activity_calendar.xml에 정의된 view 객체 중에서 id가 tv_date인것을 찾아 반환
        tvDate = (TextView)findViewById(R.id.textView_date);
        gridView = (GridView)findViewById(R.id.gridview);

        //현재 연도, 월, 일 받기
        Cal = Calendar.getInstance();
        int now_year=Cal.get(Calendar.YEAR);
        int now_month=Cal.get(Calendar.MONTH)+1;
        int day=Cal.get(Calendar.DATE);

        //현재 날짜 텍스트뷰에 뿌려줌
        tvDate.setText(now_year + "년" + now_month +"월");

        //gridview 요일 표시
        dayList = new ArrayList<String>();

        //이번달 1일 무슨요일인지 판단 Cal.set(Year,Month,Day)
        // 월 부분 -1 해주어야함 0이 1월로 잡힘
        Cal.set(now_year,now_month- 1, 1); //이번달 1일 set
        int dayNum =  Cal.get(Calendar.DAY_OF_WEEK); //1일의 요일
        // 1일 전 요일들에 공백채우기
        for (int i = 1; i < dayNum; i++) {
            dayList.add("");
        }
        //현재 월에 끝일 구하기
        setCalDate(Cal.get(Calendar.MONTH) + 1); //

        gridAdapter = new GridAdapter(getApplicationContext(), dayList); // 
        gridView.setAdapter(gridAdapter); // 어댑터를 그리스뷰 객체에 연결

    }
    // 해당 월에 표시할 일 수 구하는 메소드

    private void setCalDate(int month) {
        Cal.set(Calendar.MONTH, month - 1); //현재 월 설정

        // 현재 월의 날짜의 최대 값을 반환한다
        for (int i = 0; i < Cal.getActualMaximum(Calendar.DAY_OF_MONTH); i++) {
            dayList.add("" + (i + 1));
        }

    }

    /**
     * 그리드뷰 어댑터
     *
     */
    private class GridAdapter extends BaseAdapter {

        private List<String> list;

        private LayoutInflater inflater;

        /**
         * 생성자
         *
         * @param context
         * @param list
         */
        public GridAdapter(Context context, List<String> list) {
            this.list = list;
            this.inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        }

        @Override
        public int getCount() {

            return list.size();
        }

        @Override
        public String getItem(int position) {
            return list.get(position);
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {

            ViewHolder holder = null;

            if (convertView == null) {
                convertView = inflater.inflate(R.layout.gridview_item, parent, false);
                holder = new ViewHolder();

                holder.tvItemGridView = (TextView)convertView.findViewById(R.id.gridview_item);

                convertView.setTag(holder);
            } else {
                holder = (ViewHolder)convertView.getTag();
            }
            holder.tvItemGridView.setText("" + getItem(position));
            /*
            Cal = Calendar.getInstance();
            //오늘 day 가져옴
            Integer today = Cal.get(Calendar.DAY_OF_MONTH);
            String sToday = String.valueOf(today);  */

            return convertView;
        }
    }
    private class ViewHolder {
        TextView tvItemGridView;
    }

}
