#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <linux/errno.h>

asmlinkage long sys_sh_task_info(int pid, char *filename)
{
	struct task_struct *task;
	struct file *file;
	loff_t pos = 0;
	int fd;

	mm_segment_t old_fs = get_fs();
	set_fs(KERNEL_DS);

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
				task_pid_nr(task), \
				task->state, \
				task->prio, \
				task->rt_priority, \
				task->static_prio, \
				task->normal_prio \
			);
			
			char data[500];
			
			sprintf(data,"Process: %s\n \
				PID_Number: %ld\n \
				Process State: %ld\n \
				Priority: %ld\n \
				RT_Priority: %ld\n \
				Static Priority: %ld\n \
				Normal Priority: %ld\n",
				task->comm, \
				task_pid_nr(task), \
				task->state, \
				task->prio, \
				task->rt_priority, \
				task->static_prio, \
				task->normal_prio \
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
				return -ENOENT;
			}
		}
	}
	set_fs(old_fs);
	if(flag == 0)
	{
		return -ESRCH;
	}
	return 0;
}
