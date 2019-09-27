void Write_to_file(unsigned int fd, char* buf, size_t count)
{
	struct fd f = fdget_pos(fd);
	ssize_t ret = -EBADF;

	if (f.file) {
		loff_t pos = file_pos_read(f.file);
		ret = vfs_read(f.file, buf, count, &pos);
		if (ret >= 0)
			file_pos_write(f.file, pos);
		fdput_pos(f);
	}
}


void reverse(char str[], int length) 
{ 
    int start = 0; 
    int end = length -1; 
	char temp;
    while (start < end) 
    { 
		temp = str[start];
		str[start] = str[end];
		str[end] = temp;
        start++; 
        end--; 
    } 
} 

char* itoa(long num, char* str, int base) 
{ 
    int i = 0; 
    int isNegative = 0; 
  
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) 
    { 
        str[i++] = '0'; 
        str[i] = '\0'; 
        return str; 
    } 
  
    // In standard itoa(), negative numbers are handled only with  
    // base 10. Otherwise numbers are considered unsigned. 
    if (num < 0 && base == 10) 
    { 
        isNegative = 1; 
        num = -num; 
    } 
  
    // Process individual digits 
    while (num != 0) 
    { 
        int rem = num % base; 
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
        num = num/base; 
    } 
  
    // If number is negative, append '-' 
    if (isNegative) 
        str[i++] = '-'; 
  
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    reverse(str, i); 
  
    return str; 
}


SYSCALL_DEFINE2(sh_task_info, pid_t, PID, unsigned int, file_descriptor)
{
	int errno = 0;
	char Struct_values[1024];
	char temp_string[250];
	int num_bytes = 0;
	int PID_Found = 0;
	struct fd my_file=fdget_pos(PID);

	if(my_file.file)
	{
		struct task_struct *task_list_pointer;

		for_each_process(task_list_pointer)
		{
			if ( task_pid_nr(task_list_pointer) == (int)(PID) )
			{
				printk(KERN_DEBUG "Process: %s\n \
       					PID_Number: %ld\n \
       					Process State: %ld\n \
       					Priority: %ld\n \
       					RT_Priority: %ld\n \
       					Static Priority: %ld\n \
       					Normal Priority: %ld\n", \
       					task_list_pointer->comm, \
       					(long)task_pid_nr(task_list_pointer), \
       					(long)task_list_pointer->state, \
       					(long)task_list_pointer->prio, \
       					(long)task_list_pointer->rt_priority, \
       					(long)task_list_pointer->static_prio, \
       					(long)task_list_pointer->normal_prio \
    				);

				Write_to_file(file_descriptor, "Process: ", sizeof("Process: "));
				Write_to_file(file_descriptor,task_list_pointer->comm, 20);
				Write_to_file(file_descriptor,"\n",1);


				Write_to_file(file_descriptor, "PID_Number: ", sizeof("PID_Number: "));
				Write_to_file(file_descriptor, itoa((long)(task_pid_nr(task_list_pointer), temp_string,10), 20) ;
				Write_to_file(file_descriptor,"\n",1);

				Write_to_file(file_descriptor, "Process State: ", sizeof("Process State: "));
				Write_to_file(file_descriptor, itoa((long)(task_list_pointer->state), temp_string,10), 20) ;
				Write_to_file(file_descriptor,"\n",1);

				Write_to_file(file_descriptor, "Priority: ", sizeof("Priority: "));
				Write_to_file(file_descriptor, itoa((long)(task_list_pointer->prio),10), temp_string, 20) ;
				Write_to_file(file_descriptor,"\n",1);

				Write_to_file(file_descriptor, "RT_Priority: ", sizeof("RT_Priority: "));
				Write_to_file(file_descriptor, itoa((long)(task_list_pointer->rt_priority),10), temp_string, 20) ;
				Write_to_file(file_descriptor,"\n",1);

				Write_to_file(file_descriptor, "Static Priority: ", sizeof("Static Priority: "));
				Write_to_file(file_descriptor, itoa((long)(task_list_pointer->static_prio),10), temp_string, 20) ;
				Write_to_file(file_descriptor,"\n",1);

				Write_to_file(file_descriptor, "Normal Priority: ", sizeof("Normal Priority: "));
				Write_to_file(file_descriptor, itoa((long)(task_list_pointer->normal_prio),10), temp_string, 20) ;
				Write_to_file(file_descriptor,"\n",1);

				errno=1;
				break;
			}
			else{
				errno= -1;
			}
		}
	}
	else{
		errno=-1;
	}

		return errno;
}