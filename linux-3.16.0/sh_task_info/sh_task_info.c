#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>

asmlinkage long sys_sh_task_info(int pid, char *filename)
{
	struct task_struct *task;
	struct file *file;
	loff_t pos = 0;
	int fd;

	mm_segment_t old_fs = get_fs();
	set_fs(KERNEL_DS);

//	char* strcat1(char* , char*);

	fd = sys_open(filename, O_WRONLY|O_CREAT, 0644);
	int flag = 0;
	for_each_process(task)
	{
		if(task->pid == pid)
		{
			flag = 1;
			printk(KERN_ALERT "Process: %s\n \
				PID_Number: %ld\n \
				Process State: %ld\n \
				Priority: %ld\n \
				RT_Priority: %ld\n \
				Static Priority: %ld\n \
				Normal Priority: %ld\n ",
				task->comm, \
				(long)task_pid_nr(task), \
				(long)task->state, \
				(long)task->prio, \
				(long)task->rt_priority, \
				(long)task->static_prio, \
				(long)task->normal_prio \
			);
			char *data;
//			data = strcat1(data,task->comm);
//			data = strcat1(data,task_pid_nr(task));
//			data = strcat1(data,task->state);
//			data = strcat1(data,task->prio);
//			data = strcat1(data,task->rt_priority);
//			data = strcat1(data,task->static_prio);
//			data = strcat1(data,task->normal_prio);
			sprintf(data,"Process: %s\n \
				PID_Number: %ld\n \
				Process State: %ld\n \
				Priority: %ld\n \
				RT_Priority: %ld\n \
				Static Priority: %ld\n \
				Normal Priority: %ld\n",
				task->comm, \
				(long)task_pid_nr(task), \
				(long)task->state, \
				(long)task->prio, \
				(long)task->rt_priority, \
				(long)task->static_prio, \
				(long)task->normal_prio \
			);
			if(fd >= 0)
			{
				sys_write(fd, data, strlen(data));
				file = fget(fd);
				if(file)
				{
					vfs_write(file, data, strlen(data), &pos);
					fput(file);
				}
			sys_close(fd);
			}
			else
			{
				return -2;
			}
		}
	}
	set_fs(old_fs);
	if(flag == 0)
	{
		return -1;
	}
	return 0;
}
//char * strcat1(char *dest, char *src)
//{
//	int i,j;
//	for(i = 0; dest[i] != '\0' ; i++);
//
//	for(j = 0; src[j] != '\0' ; j++)
//	{
//		dest[i+j] = src[j];
//	}
//	dest[i+j] = '\n';
//	dest[i+j+1] = '\0';
//
//	return dest;
//}
