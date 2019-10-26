# MFC 多线程
## AfxBeginThread函数
使用方法为：msdn(https://docs.microsoft.com/en-us/cpp/parallel/multithreading-creating-worker-threads?view=vs-2019)  
```c++
// 回调函数格式
UINT MyThreadProc( LPVOID pParam );
// 调用方法
UINT MyThreadProc( LPVOID pParam )
{
    CMyObject* pObject = (CMyObject*)pParam;

    if (pObject == NULL ||
        !pObject->IsKindOf(RUNTIME_CLASS(CMyObject)))
    return 1;   // if pObject is not valid

    // do something with 'pObject'

    return 0;   // thread completed successfully
}

// inside a different function in the program
// ...
pNewObject = new CMyObject;
AfxBeginThread(MyThreadProc, pNewObject);
```
其中pParam参数可以是标量（整数，浮点数，字符等），也可以是结构体或对象。如果传入的是结构体或类对象，则当线程改变结构体或类对象中的变量时，这个变化也可以被主线程捕捉，即主线程传入回调函数的结构体貌似是一种传引用的方式。而主线程对象结构体和类对象的修改也会影响到子线程中使用对应变量的结果。

## CreateThread函数
https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
使用方法如下：
```c++
int do_perso(void* p)
{
	std::auto_ptr<perso_data> pd(reinterpret_cast<perso_data*>(p));  // 转换传入的参数

	
	// get PSN MRZ
	CEmpParams data_para;
	data_para.InputParamString(pd->data);
	CString PSN = data_para.GetParamVal("PSN");
	CString MRZ = data_para.GetParamVal("MRZ");

	//todo: add your perso process here
	int err = 0;//

	// if you want write something back to database
	{   // param_feedback yzj
		CEmpParams feedback_para;
		feedback_para.SetParam("FeedbackKey1", "FeedbackValue1"); //make sure column 'FeedbackKey1' exists in database
		feedback_para.SetParam("FeedbackKey2", "FeedbackValue2"); //make sure column 'FeedbackKey2' exists in database
		g_info_str[pd->reader_id] = feedback_para.GetParamString();

		g_msg_sender.send_feedback(pd->card_id, pd->reader_id); // begin to update values
	}


	if (0==err)
		g_msg_sender.send_perso_ok(pd->card_id, pd->reader_id);
	else
		g_msg_sender.send_perso_nk(pd->card_id, pd->reader_id);

	return err;
}
// 
// 第一个参数为 lpThreadAttributes —— 表示子线程是否继承主线程的句柄，NULL表示不继承
// 第二个参数 dwstacksize 表示给子线程分配的堆栈大小，byte为单位
// 第三个参数为调用的函数
// 第四个参数为传入调用函数的参数
// 第五个参数表示创建的线程的类型(0表示线程启动后立刻运行，为CREATE_SUSPENDED表示挂起直到被唤醒，为STACK_SIZE_PARAM_IS_A_RESERVATION指示和dwstacksize相关的信息)
// 第六个参数表示线程ID，用一个指针变量接收，为NULL表示不接收这个线程ID
CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)do_perso, pd, 0, NULL);
```

线程获取当前线程ID
```c++
DWORD id =::GetCurrentThreadId();
```